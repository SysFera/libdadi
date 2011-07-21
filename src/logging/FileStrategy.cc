#include <Logging/FileStrategy.hh>
#include <locale>
#include <sstream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>


namespace dadi {

namespace ptime = boost::posix_time;
namespace gtime = boost::gregorian;

const unsigned int RotateByTimeStrategy::EVERYDAY = 7;
const ptime::time_duration RotateByTimeStrategy::MIDNIGHT(0, 0, 0);


RotateStrategy::RotateStrategy() {}

RotateStrategy::~RotateStrategy() {}


RotateBySizeStrategy::RotateBySizeStrategy(long size) : size_(size) {}

RotateBySizeStrategy::~RotateBySizeStrategy() {}

bool
RotateBySizeStrategy::mustRotate(const std::string& path) {
  if (boost::filesystem::exists(path)) {
    return (boost::filesystem::file_size(path) >= size_);
  } else {
    return false;
  }
}

// always use UTC internally (and i mean it !)
RotateByIntervalStrategy::RotateByIntervalStrategy(
  const ptime::time_duration& td)
  : td_(td), last_(ptime::second_clock::universal_time()) {}

RotateByIntervalStrategy::~RotateByIntervalStrategy() {}

bool
RotateByIntervalStrategy::mustRotate(const std::string& path) {
  ptime::ptime current(ptime::second_clock::universal_time());
  ptime::time_duration elapsed = current - last_;

  if (td_ > elapsed ) {
    return false; // not yet
  } else {
    last_ = current; // register last rotation
    return true;
  }
}

RotateByTimeStrategy::RotateByTimeStrategy(const ptime::time_duration& td,
                                           unsigned int day)
  : td_(td), utc_(false), day_(day) {
  if ((0 > day_) && (6 < day_)) {
    day_ = EVERYDAY;
  }
}

RotateByTimeStrategy::~RotateByTimeStrategy() {}

bool
RotateByTimeStrategy::mustRotate(const std::string& path) {
  int currentDay;
  ptime::time_duration currentTime;
  // TODO: fugly
  if (utc_) {
    currentDay = gtime::day_clock::universal_day().day_of_week().as_number();
    currentTime = ptime::second_clock::universal_time().time_of_day();
  } else {
    currentDay = gtime::day_clock::local_day().day_of_week().as_number();
    currentTime = ptime::second_clock::local_time().time_of_day();
  }

  if (((day_ == currentDay) || (day_ == EVERYDAY)) &&
      (currentTime >= td_)) {
    return true;
  } else {
    return false;
  }
}

void
RotateByTimeStrategy::setLocal(bool utc) {
  utc_ = utc;
}

const std::string ArchiveByNumberStrategy::pTpl_ = "%s.%i";

ArchiveStrategy::ArchiveStrategy() {}

ArchiveStrategy::~ArchiveStrategy() {}

ArchiveByNumberStrategy::ArchiveByNumberStrategy() {}

ArchiveByNumberStrategy::~ArchiveByNumberStrategy() {}

void
ArchiveByNumberStrategy::archive(const std::string& path) {
  std::string currentPath;
  boost::format fmtr(pTpl_);
  int n(-1);
  do {
    fmtr % path % (++n);
    currentPath = fmtr.str();
  } while (boost::filesystem::exists(currentPath));

  while (n > 0) {
    fmtr % path % (--n);
    const std::string& oldPath = fmtr.str();
    boost::filesystem::rename(oldPath, currentPath);
    std::cout << "mv " << oldPath << " " << currentPath <<"\n";
    currentPath = oldPath;
  }

  boost::filesystem::rename(path, currentPath);
}

ArchiveByTimestampStrategy::ArchiveByTimestampStrategy(const std::string& tpl)
  : tpl_(tpl), local_(false) {
  using boost::posix_time::time_facet;

  time_facet* tf = new time_facet(tpl_.c_str());
  locale_.reset(new std::locale(std::locale::classic(), tf));
}

ArchiveByTimestampStrategy::~ArchiveByTimestampStrategy() {}

void
ArchiveByTimestampStrategy::setLocal(bool local) { local_ = local; }

void
ArchiveByTimestampStrategy::archive(const std::string& path) {
  using boost::posix_time::ptime;
  using boost::posix_time::microsec_clock;

  // build our timestamp
  std::ostringstream oss;

  oss.imbue(*locale_);
  ptime pt;
  if (local_) {
    pt = microsec_clock::local_time();
  } else {
    pt = microsec_clock::universal_time();
  }

  oss << pt;

  boost::format fmtr("%s.%s");
  fmtr % path % oss.str();

  std::string newPath = fmtr.str();

  if (boost::filesystem::exists(newPath)) {
    int n(-1);
    std::string currentPath;
    do {
      fmtr.parse("%s.%i");
      fmtr % newPath % (++n);
      currentPath = fmtr.str();
    } while (boost::filesystem::exists(currentPath));

    while (n > 0) {
      fmtr % newPath % (--n);
      const std::string& oldPath = fmtr.str();
      boost::filesystem::rename(oldPath, currentPath);
      std::cout << "mv " << oldPath << " " << currentPath <<"\n";
      currentPath = oldPath;
    }
  }

  boost::filesystem::rename(path, newPath);
}

} /* namespace dadi */


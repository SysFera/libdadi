#include <Logging/ArchiveStrategy.hh>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace dadi {

namespace bfs = boost::filesystem;

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
  } while (bfs::exists(currentPath));

  while (n > 0) {
    fmtr % path % (--n);
    const std::string& oldPath = fmtr.str();
    bfs::rename(oldPath, currentPath);
    std::cout << "mv " << oldPath << " " << currentPath <<"\n";
    currentPath = oldPath;
  }

  bfs::rename(path, currentPath);
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

  if (bfs::exists(newPath)) {
    int n(-1);
    std::string currentPath;
    do {
      fmtr.parse("%s.%i");
      fmtr % newPath % (++n);
      currentPath = fmtr.str();
    } while (bfs::exists(currentPath));

    while (n > 0) {
      fmtr % newPath % (--n);
      const std::string& oldPath = fmtr.str();
      bfs::rename(oldPath, currentPath);
      std::cout << "mv " << oldPath << " " << currentPath <<"\n";
      currentPath = oldPath;
    }
  }

  bfs::rename(path, newPath);
}

} /* namespace dadi */

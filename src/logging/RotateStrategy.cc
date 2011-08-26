/**
 * @file   RotateStrategy.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines log files rotate strategies
 * @section Licence
 *   |LICENCE|
 *
 */

#include "Logging/RotateStrategy.hh"
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

namespace dadi {

namespace bfs = boost::filesystem;
namespace ptime = boost::posix_time;
namespace gtime = boost::gregorian;

const unsigned int RotateByTimeStrategy::EVERYDAY = 7;
const ptime::time_duration RotateByTimeStrategy::MIDNIGHT(0, 0, 0);
// case insensitive
const boost::regex regSz("(?i)(?<nb>\\d+)(?<mul>k|m)?", boost::regex::perl);

/*****************************************************************************/

RotateStrategy::RotateStrategy() {}

RotateStrategy::~RotateStrategy() {}

/*****************************************************************************/

RotateBySizeStrategy::RotateBySizeStrategy(const std::string& size) {
  boost::smatch res;
  boost::regex_match(size, res, regSz);

  size_ = boost::lexical_cast<long>(res["nb"]);

  if (res["mul"].matched) {
    char mul = res["mul"].str()[0];
    if ('k' == mul) {
      size_ *= 1024;
    } else if ('m' == mul) {
      size_ *= 1024*1024;
    }
  }
}


RotateBySizeStrategy::~RotateBySizeStrategy() {}

bool
RotateBySizeStrategy::mustRotate(const std::string& path) {
  if (bfs::exists(path)) {
    return (bfs::file_size(path) >= size_);
  } else {
    return false;
  }
}

/*****************************************************************************/

// always use UTC internally (and i mean it !)
RotateByIntervalStrategy::RotateByIntervalStrategy(
  const ptime::time_duration& td)
  : td_(td), last_(ptime::second_clock::universal_time()) {}

RotateByIntervalStrategy::~RotateByIntervalStrategy() {}

bool
RotateByIntervalStrategy::mustRotate(const std::string& path) {
  ptime::ptime current(ptime::second_clock::universal_time());
  ptime::time_duration elapsed = current - last_;

  if (td_ > elapsed) {
    return false;  // not yet
  } else {
    last_ = current;  // register last rotation
    return true;
  }
}

/*****************************************************************************/

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

} /* namespace dadi */

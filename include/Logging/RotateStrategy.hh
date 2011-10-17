#ifndef _ROTATESTRATEGY_HH_
#define _ROTATESTRATEGY_HH_
/**
 * @file   Logging/RotateStrategy.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines log files rotate strategies
 * @section Licence
 *   |LICENCE|
 *
 */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

namespace dadi {

/**
 * @class RotateStrategy
 * @brief base class to file log rotation strategies
 */
class RotateStrategy : public boost::noncopyable {
public:
  /**
   * @brief constructor
   */
  RotateStrategy();
  /**
   * @brief destructor
   */
  virtual ~RotateStrategy();
  /**
   * @brief decide if file should be rotated
   * @warning must be reimplemented by implementors
   * @param path file (path) to be rotated
   * @return boolean (true if file must be rotated)
   */
  virtual bool
  mustRotate(const std::string& path) = 0;
};

/**
 * @class RotateBySizeStrategy
 * @brief rotation strategy based on log file size
 */
class RotateBySizeStrategy : public RotateStrategy {
public:
  /**
   * @brief constructor
   * @param size file size threshold (bytes)
   */
  explicit RotateBySizeStrategy(const std::string& size);
  /**
   * @brief destructor
   */
  ~RotateBySizeStrategy();
  /**
   * @brief decide if file should be rotated
   * @warning must be reimplemented by implementors
   * @param path file (path) to be rotated
   * @return boolean (true if file must be rotated)
   */
  bool
  mustRotate(const std::string& path);
private:
  long size_; /**< size (bytes) threshold */
};

/**
 * @class RotateByIntervalStrategy
 * @brief rotation strategy based on log file time interval
 */
class RotateByIntervalStrategy : public RotateStrategy {
public:
  /**
   * @brief constructor
   * @param td time duration between two rotation
   */
  explicit RotateByIntervalStrategy(const boost::posix_time::time_duration& td);
  /**
   * @brief destructor
   */
  ~RotateByIntervalStrategy();
  /**
   * @brief decide if file should be rotated
   * @warning must be reimplemented by implementors
   * @param path file (path) to be rotated
   * @return boolean (true if file must be rotated)
   */
  bool
  mustRotate(const std::string& path);
private:
  boost::posix_time::time_duration td_; /**< time interval between rotation */
  boost::posix_time::ptime last_; /**< last rotation time (UTC) */
};

/**
 * @class RotateByTimeStrategy
 * @brief rotation strategy based on log file age
 */
class RotateByTimeStrategy : public RotateStrategy {
public:
  /**
   * @brief constructor (by default use utc)
   * @param td time of rotation
   * @param day weekday of rotation (default: everyday)
   */
  RotateByTimeStrategy(const boost::posix_time::time_duration& td = MIDNIGHT,
                       unsigned int day = EVERYDAY);
  /**
   * @brief destructor
   */
  ~RotateByTimeStrategy();
  /**
   * @brief decide if file should be rotated
   * @warning must be reimplemented by implementors
   * @param path file (path) to be rotated
   * @return boolean (true if file must be rotated)
   */
  bool
  mustRotate(const std::string& path);
  /**
   * @brief use local time or utc
   * @param utc
   */
  void
  setLocal(bool utc = true);
private:
  static const unsigned int EVERYDAY;
  static const boost::posix_time::time_duration MIDNIGHT;
  boost::posix_time::time_duration td_;
  unsigned int day_;
  bool utc_;
};

} /* namespace dadi */


#endif /* _ROTATESTRATEGY_HH_ */

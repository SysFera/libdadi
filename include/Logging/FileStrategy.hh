#ifndef _FILESTRATEGY_HH_
#define _FILESTRATEGY_HH_

#include <string>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace dadi {

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
  virtual bool mustRotate(const std::string& path) = 0;
};

class RotateBySizeStrategy : public RotateStrategy {
public:
  /**
   * @brief constructor
   * @param size file size threshold (bytes)
   */
  RotateBySizeStrategy(long size);
  ~RotateBySizeStrategy();
  bool mustRotate(const std::string& path);
private:
  long size_;
};

class RotateByIntervalStrategy : public RotateStrategy {
public:
  /**
   * @brief constructor
   * @param td time duration between two rotation
   */
  RotateByIntervalStrategy(const boost::posix_time::time_duration& td);
  ~RotateByIntervalStrategy();
  bool mustRotate(const std::string& path);
private:
  boost::posix_time::time_duration td_; /**< time interval between rotation */
  boost::posix_time::ptime last_; /**< last rotation time (UTC) */
};

class RotateByTimeStrategy : public RotateStrategy {
public:
  /**
   * @brief constructor (by default use utc)
   * @param td time of rotation
   * @param day weekday of rotation (default: everyday)
   */
  RotateByTimeStrategy(const boost::posix_time::time_duration& td = MIDNIGHT,
                       unsigned int day = EVERYDAY);
  ~RotateByTimeStrategy();
  bool mustRotate(const std::string& path);
  /**
   * @brief use local time or utc
   * @param utc
   */
  void setLocal(bool utc = true);
private:
  static const unsigned int EVERYDAY;
  static const boost::posix_time::time_duration MIDNIGHT;
  boost::posix_time::time_duration td_;
  unsigned int day_;
  bool utc_;
};



class ArchiveStrategy : public boost::noncopyable {
public:
  /**
   * @brief constructor
   */
  ArchiveStrategy();
  /**
   * @brief destructor
   */
  virtual ~ArchiveStrategy();
  /**
   * @brief do the actual archiving
   * @warning must be reimplemented by implementors
   * @param path file to be archived
   */
  virtual void archive(const std::string& path) = 0;
};

class ArchiveByNumberStrategy : public ArchiveStrategy {
public:
  ArchiveByNumberStrategy();
  ~ArchiveByNumberStrategy();
  void archive(const std::string& path);
protected:
  static const std::string pTpl_;
};

class ArchiveByTimestampStrategy : public ArchiveStrategy {
public:
  /**
   * @brief constructor (by default use utc)
   * @param tpl timestamp template (default: YYYYMMDDhhmmss)
   */
  ArchiveByTimestampStrategy(const std::string& tpl = "%Y%m%d%H%M%S");
  ~ArchiveByTimestampStrategy();
  /**
   * @brief use local time or utc
   * @param local
   */
  void setLocal(bool local);
  void archive(const std::string& path);
private:
  const std::string tpl_;
  boost::scoped_ptr<std::locale> locale_; /**< locale containing our time_facet */
  bool local_; /**< use local time or utc (utc by default) */
};

} /* namespace dadi */

#endif /* _FILESTRATEGY_HH_ */

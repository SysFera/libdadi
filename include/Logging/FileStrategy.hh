#ifndef _FILESTRATEGY_HH_
#define _FILESTRATEGY_HH_

#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/format.hpp>
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
  explicit RotateBySizeStrategy(long size);
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
  explicit RotateByIntervalStrategy(const boost::posix_time::time_duration& td);
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
  explicit ArchiveByTimestampStrategy(const std::string& tpl =
                                      "%Y%m%d%H%M%S");
  ~ArchiveByTimestampStrategy();
  /**
   * @brief use local time or utc
   * @param local
   */
  void setLocal(bool local);
  void archive(const std::string& path);
private:
  const std::string tpl_;
  boost::scoped_ptr<std::locale> locale_; /**< locale with custom time_facet */
  bool local_; /**< use local time or utc (utc by default) */
};

class PurgeStrategy : boost::noncopyable {
public:
  /**
   * @brief constructor
   */
  PurgeStrategy();
  /**
   * @brief destructor
   */
  virtual ~PurgeStrategy();
  /**
   * @brief purge archived file
   * @warning must be reimplemented by implementors
   * @param path file (path) to be purged
   */
  virtual void purge(const std::string& path) = 0;
protected:
  /**
   * @brief list all archive files linked to log file
   * @param[in] basename log file (path)
   * @param[out] paths archived files
   */
  void list(const std::string& basename,
            std::vector<std::string>& paths);
  /**
   * @brief sort archives to ease
   * @warning needs to be reimplemented (does nothing by default)
   * @param paths files to be sorted
   */
  virtual void sort(std::vector<std::string>& paths);
  static boost::format regexTpl; /**< regex template to filter archives */
};

// ensure a maximum number of archives and delete oldest
class PurgeByCountStrategy : public PurgeStrategy {
public:
  /**
   * @brief constructor
   * @param count number of archives to keep
   */
  explicit PurgeByCountStrategy(int count);
  ~PurgeByCountStrategy();

  void purge(const std::string& path);
protected:
  void sort(std::vector<std::string>& paths);
private:
  int count_;
};



} /* namespace dadi */

#endif /* _FILESTRATEGY_HH_ */

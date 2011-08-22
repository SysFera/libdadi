#ifndef _PURGESTRATEGY_HH_
#define _PURGESTRATEGY_HH_

#include <string>
#include <vector>
#include <boost/format.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/scoped_ptr.hpp>

namespace dadi {

/**
 * @class PurgeStrategy
 * @brief Base class to purge strategies
 */
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
   * @brief purge archived files
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

/*****************************************************************************/

/**
 * @class PurgeByCountStrategy
 * @brief ensure a maximum number of archives and delete oldest
 */
class PurgeByCountStrategy : public PurgeStrategy {
public:
  /**
   * @brief constructor
   * @param count number of archives to keep
   */
  explicit PurgeByCountStrategy(int count);
  /**
   * @brief destructor
   */
  ~PurgeByCountStrategy();

  /**
   * @brief purge archived files
   * @warning must be reimplemented by implementors
   * @param path file (path) to be purged
   */
  void purge(const std::string& path);
protected:
  void sort(std::vector<std::string>& paths);
private:
  int count_; /**< number of archiches to keep */
};

} /* namespace dadi */

#endif /* _PURGESTRATEGY_HH_ */

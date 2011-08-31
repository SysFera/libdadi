#ifndef _ARCHIVESTRATEGY_HH_
#define _ARCHIVESTRATEGY_HH_
/**
 * @file   Logging/ArchiveStrategy.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines log files archive strategies
 * @section Licence
 *   |LICENCE|
 *
 */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace dadi {

/**
 * @class ArchiveStrategy
 * @brief archiving strategies base class (don't use it directly)
 */
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

/*****************************************************************************/

/**
 * @class ArchiveByNumberStrategy
 * @brief implement archiving by number stategy
 */
class ArchiveByNumberStrategy : public ArchiveStrategy {
public:
  /**
   * @brief constructor
   */
  ArchiveByNumberStrategy();
  /**
   * @brief destructor
   */
  ~ArchiveByNumberStrategy();
  /**
   * @brief archive current log file
   * @param path current log file path
   */
  void archive(const std::string& path);
protected:
  static const std::string pTpl_; /**< template archive filename */
};

/*****************************************************************************/

/**
 * @class ArchiveByTimestampStrategy
 * @brief implement archiving timestamp stategy
 */
class ArchiveByTimestampStrategy : public ArchiveStrategy {
public:
  /**
   * @brief constructor (by default use utc)
   * @param tpl timestamp template (default: YYYYMMDDhhmmss)
   */
  explicit ArchiveByTimestampStrategy(const std::string& tpl =
                                      "%Y%m%d%H%M%S");
  /**
   * @brief destructor
   */
  ~ArchiveByTimestampStrategy();

  /**
   * @brief use local time or utc
   * @param local
   */
  void setLocal(bool local);
  /**
   * @brief archive current log filex
   * @param path current log file path
   */
  void archive(const std::string& path);
private:
  const std::string tpl_;
  boost::scoped_ptr<std::locale> locale_; /**< locale with custom time_facet */
  bool local_; /**< use local time or utc (utc by default) */
};

} /* namespace dadi */

#endif /* _ARCHIVESTRATEGY_HH_ */

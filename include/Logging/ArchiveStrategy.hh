#ifndef _ARCHIVESTRATEGY_HH_
#define _ARCHIVESTRATEGY_HH_
/**
 * @file   ArchiveStrategy.hh
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

class ArchiveByNumberStrategy : public ArchiveStrategy {
public:
  ArchiveByNumberStrategy();
  ~ArchiveByNumberStrategy();
  void archive(const std::string& path);
protected:
  static const std::string pTpl_;
};

/*****************************************************************************/

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

} /* namespace dadi */

#endif /* _ARCHIVESTRATEGY_HH_ */

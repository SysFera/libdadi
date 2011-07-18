#ifndef _FILESTRATEGY_HH_
#define _FILESTRATEGY_HH_

#include <locale>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

class RotateStrategy : public boost::noncopyable {
public:
  RotateStrategy();
  virtual ~RotateStrategy();
  virtual bool mustRotate(const std::string& path) = 0;
};

class RotateBySizeStrategy : public RotateStrategy {
public:
  RotateBySizeStrategy(long size);
  ~RotateBySizeStrategy();
  bool mustRotate(const std::string& path);
private:
  long size_;
};

class ArchiveStrategy : public boost::noncopyable {
public:
  ArchiveStrategy();
  virtual ~ArchiveStrategy();
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
   * @brief constructor
   * @param tpl timestamp template (default: YYYYMMDDhhmmss)
   */
  ArchiveByTimestampStrategy(const std::string& tpl = "%Y%m%d%H%M%S");
  ~ArchiveByTimestampStrategy();

  void setLocal(bool local);
  void archive(const std::string& path);
private:
  const std::string tpl_;
  boost::scoped_ptr<std::locale> locale_; /**< locale containing our time_facet */
  bool local_; /**< use local time or utc (utc by default) */
};



#endif /* _FILESTRATEGY_HH_ */

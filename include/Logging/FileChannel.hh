#ifndef _FILECHANNEL_HH_
#define _FILECHANNEL_HH_

#include "Channel.hh"
#include <map>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/regex_fwd.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include "FileStrategy.hh"

namespace dadi {

/**
 * properties:
 * path: log file path
 * compression type: none, bzip, gzip, zlib
 *   (currently an int, but we should think about enabling user to use strings)
 * archive: none, number, timestamp
 * rotate: none, size, interval (format: HH:mm:ss), time (format: [day,]HH:mm:ss)
 * purge: none, count, age
 */
class FileChannel : public Channel {
public:
  enum CompressionMode {
    COMP_NONE=0,
    COMP_BZIP2,
    COMP_GZIP,
    COMP_ZLIB
  };

  enum ArchiveMode {
    AR_NONE=0,
    AR_NUMBER,
    AR_TIMESTAMP
  };

  enum RotateMode {
    ROT_NONE=0,
    ROT_SIZE,
    ROT_INTERVAL,
    ROT_TIME
  };

  enum PurgeMode {
    PURGE_NONE=0,
    PURGE_COUNT,
    PURGE_AGE
  };

  FileChannel();
  FileChannel(const std::string& path);
  virtual ~FileChannel();

  void open();
  void close();
  void log(const Message& msg);

  long getLastWriteTime() const;
  // return file size in bytes (b*te me i'm famous)
  long getSize() const;
  const std::string& getPath() const;
protected:
  static const std::string ATTR_PATH;
  static const std::string ATTR_COMPRESSION_MODE;
  static const std::string ATTR_ARCHIVE;
  static const std::string ATTR_ROTATE;
  static const std::string ATTR_ROTATE_SIZE;
  static const std::string ATTR_ROTATE_TIME;
  static const std::string ATTR_ROTATE_INTERVAL;
  static const std::string ATTR_PURGE;
  static const std::string ATTR_PURGE_COUNT;
  // filter rotate.interval when using time rotate policy
  static const boost::regex regex1;
  static std::map<std::string, int> attrMap;

  void setStream();
  void setArchiveStrategy();
  void setRotateStrategy();
  void setPurgeStrategy();

  void purge();
private:
  std::string path_;
  boost::scoped_ptr<RotateStrategy> pRotateStrategy_;
  boost::scoped_ptr<ArchiveStrategy> pArchiveStrategy_;
  boost::scoped_ptr<PurgeStrategy> pPurgeStrategy_;
  boost::iostreams::filtering_ostream out_;
  boost::mutex mutex_;
};

} /* namespace dadi */

#endif /* _FILECHANNEL_HH_ */

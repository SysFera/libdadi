#ifndef _FILECHANNEL_HH_
#define _FILECHANNEL_HH_
/**
 * @file   FileChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines a file channel
 *
 */

#include "Channel.hh"
#include <string>
#include <map>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/regex_fwd.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include "FileStrategy.hh"

namespace dadi {

/**
 * @class FileChannel
 * @brief Channel that logs into a file
 *
 * This Channel allows log rotation, archiving and archives automatic purging
 * By default, it logs messages followed by a newline.
 * It is configured through properties.
 * Properties could be set by using the putAttr method.
 *
 * properties supported:
 * - path: log file path **mandatory**
 * - compression_mode: values allowed (none, bzip, gzip, zlib)
 * - archive: values allowed (none, number, timestamp)
 * - rotate: values allowed (none, size, interval (format: HH:mm:ss))
 * - rotate.size: maximum file size (bytes)
 * - rotate.time: values allowed (utc, time)
 * - rotate.interval: (format: [day,]HH:mm:ss)
 * - purge: values allowed (none, count, age)
 * - purge.count: maximum number of archives
 */
class FileChannel : public Channel {
public:
  /**
   * @enum CompressionMode
   */
  enum CompressionMode {
    COMP_NONE = 0, /**< no compression */
    COMP_BZIP2, /**< bzip2 compression */
    COMP_GZIP, /**< gzip compression */
    COMP_ZLIB /**< zlib compression */
  };

  /**
   * @enum ArchiveMode
   */
  enum ArchiveMode {
    AR_NONE = 0, /**< no archiving */
    AR_NUMBER, /**< archive filename pattern: <filename>.<number++> */
    AR_TIMESTAMP /**< archive filename pattern: <filename>.<timestamp> */
  };

  /**
   * @enum RotateMode
   */
  enum RotateMode {
    ROT_NONE = 0, /**< no rotation */
    ROT_SIZE, /**< rotation based on file size */
    ROT_INTERVAL, /**< rotation based on time interval */
    ROT_TIME, /**< rotation based on fixed time */
    ROT_LINE /**< rotation based on line number @todo unimplemented */
  };

  /**
   * @enum PurgeMode
   */
  enum PurgeMode {
    PURGE_NONE = 0, /**< no purging bébé */
    PURGE_COUNT, /**< purging based on archives number */
    PURGE_AGE /**< purging based on file age */
  };

  /**
   * @brief default constructor
   * @warning you need to set log file path either by using the appropriate
   * constructor or by setting in the attributes (key: "path")
   */
  FileChannel();
  /**
   * @brief constructor
   * @param path log file path
   */
  explicit FileChannel(const std::string& path);
  /**
   * @brief destructor
   */
  virtual ~FileChannel();

  /**
   * @brief open channel
   */
  void open();
  /**
   * @brief close channel
   */
  void close();
  /**
   * @brief log message
   * @param msg message to be logged
   */
  void log(const Message& msg);

  /**
   * @brief get last modification time
   * @return timestamp or -1 if log file does not exist
   */
  long getLastWriteTime() const;
  /**
   * @brief get file size
   * @return file size in bytes (b*te me i'm famous)
   */
  long getSize() const;
  /**
   * @brief get log file path
   * @return file path
   */
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
  static const boost::regex regex1; /**< regular expression @internal */
  static std::map<std::string, int> attrMap; /**< properties map */

  /**
   * @brief set archive strategy
   */
  void setArchiveStrategy();
  /**
   * @brief set rotation strategy
   */
  void setRotateStrategy();
  /**
   * @brief set purge strategy
   */
  void setPurgeStrategy();

  /**
   * @brief purge archives
   */
  void purge();
private:
  std::string path_; /**< log file path */
  boost::scoped_ptr<RotateStrategy> pRotateStrategy_; /**< rotation strategy */
  boost::scoped_ptr<ArchiveStrategy> pArchiveStrategy_; /**< archive strategy */
  boost::scoped_ptr<PurgeStrategy> pPurgeStrategy_; /**< purge strategy */
  boost::iostreams::filtering_ostream out_; /**< log file stream */
  boost::mutex mutex_; /**< mutex protecting concurrent access */
};

} /* namespace dadi */

#endif /* _FILECHANNEL_HH_ */

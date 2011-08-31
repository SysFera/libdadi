#ifndef _MESSAGE_HH_
#define _MESSAGE_HH_
/**
 * @file   Logging/Message.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines log message
 * @section Licence
 *   |LICENCE|
 *
 */

#include <map>
#include <string>

namespace dadi {

/** map of std::string/std::strings hold properties */
typedef std::map<std::string, std::string> StringMap;

/**
 * @class Message
 * @brief log message
 */
class Message {
public:
  /**
   * @enum Priority
   * @brief logging priorities
   */
  enum Priority {
    PRIO_TRACE = 1,   /**< trace logs */
    PRIO_DEBUG,       /**< debug logs */
    PRIO_INFORMATION, /**< information logs */
    PRIO_WARNING,     /**< warning logs */
    PRIO_ERROR,       /**< error logs */
    PRIO_CRITICAL,    /**< critical logs */
    PRIO_FATAL        /**< fatal logs */
  };

  /**
   * @brief default constructor
   */
  Message();
  /**
   * @brief constructor
   * @param src source
   * @param txt message
   * @param prio priority
   */
  Message(const std::string& src,
          const std::string& txt,
          Priority prio);
  /**
   * @brief constructor
   * @param src source
   * @param txt message
   * @param prio priority
   * @param file source file name
   * @param line source file line
   */
  Message(const std::string& src,
          const std::string& txt,
          Priority prio,
          const std::string& file,
          int line);

  /**
   * @brief set log message source
   * @param src
   */
  void setSource(const std::string& src);
  /**
   * @brief get log message source
   * @return source
   */
  const std::string& getSource() const;

  /**
   * @brief set log message text
   * @param txt log message
   */
  void setText(const std::string& txt);
  /**
   * @brief get log message text
   * @return log message
   */
  const std::string& getText() const;

  /**
   * @brief set log message priority
   * @param prio priority
   */
  void setPriority(Priority prio);
  /**
   * @brief get log message priority
   * @return priority
   */
  Priority getPriority() const;

  /**
   * @brief set log message timestamp
   * @param timestamp
   */
  void setTimestamp(long timestamp);
  /**
   * @brief get log message timestamp
   * @return timestamp
   */
  long getTimestamp() const;

  /**
   * @brief set source code filename
   * @param filename source code filename
   */
  void setFile(const std::string& filename);
  /**
   * @brief get source code filename
   * @return source code filename
   */
  const std::string& getFile() const;

  /**
   * @brief set source code line number
   * @param line source code line number
   */
  void setLine(int line);
  /**
   * @brief get source code line number
   * @return source code line number
   */
  int getLine() const;

  /**
   * @brief get attribute (write access)
   * @param key
   * @return attribute value
   */
  std::string& operator[](const std::string& key);
  /**
   * @brief get attribute (read access)
   * @param key
   * @return attribute value
   */
  const std::string& operator[](const std::string& key) const;

private:
  std::string src_; /**< log message source */
  std::string txt_; /**< log message test */
  Priority prio_; /**< log message priority */
  long timestamp_; /**< log message timestamp */
  std::string file_; /**< source code filename */
  int line_; /**< source code line number */
  mutable StringMap tags_; /**< FileChannel properties map */
};

} /* namespace dadi */

#endif /* _MESSAGE_HH_ */

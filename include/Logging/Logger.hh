#ifndef _LOGGER_HH_
#define _LOGGER_HH_

#include "Channel.hh"
#include <map>
#include <string>
#include <vector>
#include <boost/thread/recursive_mutex.hpp>

namespace dadi {

class Logger;
typedef boost::shared_ptr<Logger> LoggerPtr;
typedef std::map<std::string, LoggerPtr> LoggerMap;

/**
 * @class Logger
 * @brief Logger log4j style API
 *
 * Logger acts as the entry point of libdadi logging framework.
 * Loggers are organized in a tree-fashionned hierarchy and are
 * obtained through static methods (ie: getLogger).
 * There is a special node called rootLogger (channel: null,
 * priority: PRIO_INFORMATION)
 * Logger instances inherit from their parent their Channel and priority.
 *
 * Application uses Logger instances to send log messages
 * by the intermediate of the registered Channel instance.
 * Each Logger has a minimum logging level so that only messages
 * with priority equal or higher will be effectively reported.
 */
class Logger : public Channel {
public:
  /**
   * @brief get logger name
   * @return this logger name
   * @warning loggers are recursively named following this pattern:
   * <parent>.<logger>. Root logger is named "".
   */
  const std::string& getName() const;

  /**
   * @brief set Logger Channel
   * @param channel ChannelPtr
   */
  void setChannel(ChannelPtr channel);
  /**
   * @brief get Logger Channel
   * @return ChannelPtr
   */
  ChannelPtr getChannel() const;

  /**
   * @brief set logger threshold
   * Only message with a priority level superior or equal to level
   * will be logged.
   * @param level minimum level of logging
   * @warning level must be between [PRIO_TRACE, PRIO_FATAL] if not,
   * it will be set by default at PRIO_INFORMATION
   */
  void setLevel(int level);
  /**
   * @brief get threshold
   * @return minimum level of logging
   */
  int getLevel() const;

  /**
   * @brief logs Message
   * @param msg message to log
   */
  void log(const Message& msg);

  /**
   * @brief checks if logger will effectively log messages with
   * priority >= level
   * @param level level to be checked
   */
  bool is(int level) const;
  /**
   * @brief alias to is(Message::PRIO_TRACE)
   */
  bool trace() const;
  /**
   * @brief alias to is(Message::PRIO_DEBUG)
   */
  bool debug() const;
  /**
   * @brief alias to is(Message::PRIO_INFORMATION)
   */
  bool information() const;
  /**
   * @brief alias to is(Message::PRIO_WARNING)
   */
  bool warning() const;
  /**
   * @brief alias to is(Message::PRIO_ERROR)
   */
  bool error() const;
  /**
   * @brief alias to is(Message::PRIO_CRITICAL)
   */
  bool critical() const;
  /**
   * @brief alias to is(Message::PRIO_FATAL)
   */
  bool fatal() const;

  /**
   * @brief get root Logger
   * @return LoggerPtr to the root Logger
   */
  static LoggerPtr getRootLogger();
  /**
   * @brief get Logger (or create if needed)
   * @param name Logger name
   * @return LoggerPtr
   */
  static LoggerPtr getLogger(const std::string& name);
  /**
   * @brief check if logger is registered
   * @param name logger name
   * @return boolean
   */
  static bool hasLogger(const std::string& name);
  /**
   * @brief create a Logger with specified name, channel and level
   * since it returns a boost::shared_ptr, you can test if it is
   * valid or not
   *
   * @param name Logger name
   * @param channel Channel attached to Logger
   * @param level threshold
   * @return LoggerPtr
   */
  static LoggerPtr createLogger(const std::string& name,
                                ChannelPtr channel,
                                int level);
  /**
   * @brief destroy a logger and all its children
   * @param name
   * @warning this method unregister the logger from the map
   * but as long as you hold a pointer on the logger, it won't
   * be deleted
   */
  static void destroyLogger(const std::string& name);
  /**
   * @brief shutdown the logging hierarchy
   */
  static void shutdown();
  /**
   * @brief get all active Logger names
   * @param[out] names vector where Logger names will be stored
   */
  static void getActiveLoggers(std::vector<std::string>& names);

  static const std::string root_; /**< root logger name */
protected:
  /**
   * @brief construct Logger
   * @param name Logger name
   * @param channel Channel attached to Logger
   * @param level threshold
   */
  Logger(const std::string& name,
         ChannelPtr channel,
         int level);
  /**
   * @brief find a LoggerPtr in the hierarchy
   * @param name Logger name
   * @return LoggerPtr
   */
  static LoggerPtr find(const std::string& name);
  /**
   * @brief get Logger
   * @param name Logger name
   * @return LoggerPtr
   */
  static LoggerPtr get(const std::string& name);
  /**
   * @brief add Logger to hierarchy
   * @param logger LoggerPtr
   */
  static void add(LoggerPtr logger);
  /**
   * @brief return Logger parent
   * @param name Logger name
   * @return LoggerPtr
   */
  static LoggerPtr getParent(const std::string& name);

private:
  std::string name_; /**< logger name */
  ChannelPtr channel_; /**< destination channel for logs */
  int level_; /**< minimum level to log */
  static LoggerMap lmap_; /**< logger map */
  static boost::recursive_mutex mutex_; /**< mutex protecting lmap_ access */
};

} /* namespace dadi */

#endif /* _LOGGER_HH_ */

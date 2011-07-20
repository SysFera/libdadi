#ifndef _LOGGER_HH_
#define _LOGGER_HH_

#include "Channel.hh"
#include <map>
#include <vector>
#include <boost/thread/recursive_mutex.hpp>

namespace dadi {

class Logger;
typedef boost::shared_ptr<Logger> LoggerPtr;
typedef std::map<std::string, LoggerPtr> LoggerMap;


class Logger : public Channel {
public:
  /**
   * @brief get logger name
   * @return this logger name
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
   * @brief set threshold
   * @param level minimum level of logging
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

  bool is(int level) const;
  bool trace() const;
  bool debug() const;
  bool information() const;
  bool warning() const;
  bool error() const;
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
   * @brief create a Logger with specified name, channel and level
   * since it returns a boost::shared_ptr, you can test if it's valid or not
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

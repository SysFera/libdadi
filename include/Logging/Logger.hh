#ifndef _LOGGER_HH_
#define _LOGGER_HH_

#include "Channel.hh"
#include <map>

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
   * @brief set channel
   * @param channel channel to push our logs
   */
  void setChannel(ChannelPtr channel);
  /**
   * @brief get underlying channel
   * @return underlying channel
   */
  ChannelPtr getChannel() const;

  void setLevel(int level);
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

  static LoggerPtr getRootLogger();
  static LoggerPtr getLogger(const std::string& name);
  static LoggerPtr createLogger(const std::string& name,
                                ChannelPtr channel,
                                int level);
  /**
   * @brief shutdown the logging hierarchy
   */
  static void shutdown();
  static const std::string root_; /**< root logger name */

protected:
  Logger(const std::string& name,
         ChannelPtr channel,
         int level);
  static LoggerPtr find(const std::string& name);
  static LoggerPtr get(const std::string& name);
  static void add(LoggerPtr logger);
  static LoggerPtr getParent(const std::string& name);

private:
  std::string name_; /**< logger name */
  ChannelPtr channel_; /**< destination channel for logs */
  int level_; /**< minimum level to log */
  static LoggerMap lmap_; /**< logger map */
};

} /* namespace dadi */

#endif /* _LOGGER_HH_ */

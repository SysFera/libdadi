/**
 * @file   Logging/SyslogChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines syslog channel
 * @section Licence
 *   |LICENCE|
 *
 */

#ifndef _SYSLOGCHANNEL_HH_
#define _SYSLOGCHANNEL_HH_

#include <string>
#include "dadi/Logging/Channel.hh"

namespace dadi {


class SyslogChannel : public Channel {
public:
  /**
   * @enum Option
   * @brief syslog options
   * @see man 3p openlog
   * @see syslog.h (from 4.4BSD)
   */
  enum Option {
    SYSLOG_PID = 0x01,
    SYSLOG_CONS = 0x02,
    SYSLOG_ODELAY = 0x04,
    SYSLOG_NDELAY = 0x08,
    SYSLOG_NOWAIT = 0x10,
    SYSLOG_PERROR = 0x20
  };

  /**
   * @enum Facility
   * @brief syslog facilities
   */
  enum Facility {
    SYSLOG_KERN = (0<<3),
    SYSLOG_USER = (1<<3),
    SYSLOG_MAIL = (2<<3),
    SYSLOG_DAEMON = (3<<3),
    SYSLOG_AUTH = (4<<3),
    SYSLOG_SYSLOG = (5<<3),
    SYSLOG_LPR = (6<<3),
    SYSLOG_NEWS = (7<<3),
    SYSLOG_UUCP = (8<<3),
    SYSLOG_CRON = (9<<3),
    SYSLOG_AUTHPRIV = (10<<3),
    SYSLOG_FTP = (11<<3),
    SYSLOG_LOCAL0 = (16<<3),
    SYSLOG_LOCAL1 = (17<<3),
    SYSLOG_LOCAL2 = (18<<3),
    SYSLOG_LOCAL3 = (19<<3),
    SYSLOG_LOCAL4 = (20<<3),
    SYSLOG_LOCAL5 = (21<<3),
    SYSLOG_LOCAL6 = (22<<3),
    SYSLOG_LOCAL7 = (23<<3)
  };

  /**
   * @brief default constructor
   */
  SyslogChannel();
  /**
   * @brief constructor
   * @param name channel name
   * @param option syslog options
   * @param facility syslog facilities
   */
  SyslogChannel(const std::string& name,
                unsigned int option,
                unsigned int facility);

  /**
   * @brief destructor
   */
  ~SyslogChannel();

  /**
   * @brief open channel
   */
  void
  open();
  /**
   * @brief close channel
   */
  void
  close();
  /**
   * @brief logs message
   * @param msg Message to log
   */
  void
  log(const Message& msg);

protected:
  /**
   * @brief get channel current priority
   * @param msg Message to log
   * @return priority level
   */
  int
  getPrio(const Message& msg);

  static const std::string ATTR_NAME; /**< attribute name key */
  static const std::string ATTR_FACILITY; /**< attribute facility key */
  static const std::string ATTR_OPTION; /**< attribute option key */

private:
  std::string name_; /**< channel name */
  int option_; /**< syslog option */
  int facility_; /**< syslog facility */
  bool open_; /**< keep channel state (open/closed) */
};


} /* namespace dadi */

#endif /* _SYSLOGCHANNEL_HH_ */

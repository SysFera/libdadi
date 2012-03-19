/**
 * @file   Logging/SyslogChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines syslog channel
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _SYSLOGCHANNEL_HH_
#define _SYSLOGCHANNEL_HH_

#include <string>
#include "dadi/Logging/Channel.hh"

namespace dadi {


/**
 * @class SyslogChannel
 * @brief Channel that logs into syslogs
 */
class SyslogChannel : public Channel {
public:
  /**
   * @enum Option
   * @brief syslog options
   * @see man 3p openlog
   * @see syslog.h (from 4.4BSD)
   */
  enum Option {
    /** log the pid with each message */
    SYSLOG_PID = 0x01,
    /** log on the console if errors in sending */
    SYSLOG_CONS = 0x02,
    /** delay open until first syslog() (default) */
    SYSLOG_ODELAY = 0x04,
    /** don't delay open */
    SYSLOG_NDELAY = 0x08,
    /** don't wait for console forks: DEPRECATED */
    SYSLOG_NOWAIT = 0x10,
    /** log to stderr as well */
    SYSLOG_PERROR = 0x20
  };

  /**
   * @enum Facility
   * @brief syslog facilities
   */
  enum Facility {
    /** kernel messages */
    SYSLOG_KERN = (0<<3),
    /** random user-level messages */
    SYSLOG_USER = (1<<3),
    /** mail system */
    SYSLOG_MAIL = (2<<3),
    /** system daemons */
    SYSLOG_DAEMON = (3<<3),
    /** security/authorization messages */
    SYSLOG_AUTH = (4<<3),
    /** messages generated internally by syslogd */
    SYSLOG_SYSLOG = (5<<3),
    /** line printer subsystem */
    SYSLOG_LPR = (6<<3),
    /** network news subsystem */
    SYSLOG_NEWS = (7<<3),
    /** UUCP subsystem */
    SYSLOG_UUCP = (8<<3),
    /** clock daemon */
    SYSLOG_CRON = (9<<3),
    /** security/authorization messages (private) */
    SYSLOG_AUTHPRIV = (10<<3),
    /** ftp daemon */
    SYSLOG_FTP = (11<<3),
    /** reserved for local use */
    SYSLOG_LOCAL0 = (16<<3),
    /** reserved for local use */
    SYSLOG_LOCAL1 = (17<<3),
    /** reserved for local use */
    SYSLOG_LOCAL2 = (18<<3),
    /** reserved for local use */
    SYSLOG_LOCAL3 = (19<<3),
    /** reserved for local use */
    SYSLOG_LOCAL4 = (20<<3),
    /** reserved for local use */
    SYSLOG_LOCAL5 = (21<<3),
    /** reserved for local use */
    SYSLOG_LOCAL6 = (22<<3),
    /** reserved for local use */
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


  SyslogChannel(const std::string& name,                unsigned int option,
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

  static const std::string ATTR_NAME;  /**< attribute name key */
  static const std::string ATTR_FACILITY;  /**< attribute facility key */
  static const std::string ATTR_OPTION;  /**< attribute option key */

private:
  std::string name_; /**< channel name */
  int option_; /**< syslog option */
  int facility_; /**< syslog facility */
  bool open_; /**< keep channel state (open/closed) */
};


} /* namespace dadi */

#endif  /* _SYSLOGCHANNEL_HH_ */

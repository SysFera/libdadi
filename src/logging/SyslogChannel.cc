#include <Logging/SyslogChannel.hh>
#include <syslog.h>
#include <Logging/Message.hh>

namespace dadi {

const std::string SyslogChannel::ATTR_NAME = "name";
const std::string SyslogChannel::ATTR_OPTION = "option";
const std::string SyslogChannel::ATTR_FACILITY = "facility";

SyslogChannel::SyslogChannel() :
  option_(SYSLOG_CONS), facility_(SYSLOG_USER), open_(false) {}

SyslogChannel::SyslogChannel(const std::string& name,
                             unsigned int option,
                             unsigned int facility) :
  name_(name), option_(option), facility_(facility), open_(false) {}

SyslogChannel::~SyslogChannel() {
  if (open_) {
    close();
    open_ = false;
  }
}

void
SyslogChannel::open() {
  if (name_.empty()) {
    try {
      name_ = getAttr<std::string>(ATTR_NAME);
    } catch (...) {}
  }

  try {
    unsigned int tmp = getAttr<unsigned int>(ATTR_OPTION);
    option_ = tmp;
  } catch (...) {}

  try {
    unsigned int tmp = getAttr<unsigned int>(ATTR_FACILITY);
    facility_ = tmp;
  } catch (...) {}

  openlog(name_.c_str(), option_, facility_);
  open_ = true;
}

void
SyslogChannel::close() {
  if (open_) {
    closelog();  // according man page, this is optional
  }
}

void
SyslogChannel::log(const Message& msg) {
  if (!open_) {
    open();
  }
  syslog(getPrio(msg), "%s", msg.getText().c_str());
}


int
SyslogChannel::getPrio(const Message& msg) {
  switch (msg.getPriority()) {
  case Message::PRIO_TRACE:
  case Message::PRIO_DEBUG:
    return LOG_DEBUG;
  case Message::PRIO_INFORMATION:
    return LOG_INFO;
  case Message::PRIO_WARNING:
    return LOG_WARNING;
  case Message::PRIO_ERROR:
    return LOG_ERR;
  case Message::PRIO_CRITICAL:
    return LOG_CRIT;
  case Message::PRIO_FATAL:
    return LOG_ALERT;
  default:
    return 0;
  }
}


} /* namespace dadi */

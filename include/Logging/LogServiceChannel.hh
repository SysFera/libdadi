#ifndef _LOGSERVICECHANNEL_HH_
#define _LOGSERVICECHANNEL_HH_

#include "Channel.hh"
#include "LogComponentBase.hh"
#include "LogORBMgr.hh"

namespace dadi {

class LogServiceChannel : public Channel {
public:

  /**
   * @brief constructor
   * @param argc number of command line arguments
   * @param argv array of command line arguments
   * TODO: allow LogServiceChannel to get command line arguments from global store
   * so we could have a sane default constructor
   */
  LogServiceChannel(int argc, char *argv[]);
  ~LogServiceChannel();

  void open();
  void close();
  void log(const Message& msg);
private:
  LogComponentBase *lb;
};



} /* namespace dadi */


#endif /* _LOGSERVICECHANNEL_HH_ */

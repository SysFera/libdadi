#ifndef _LOGSERVICECHANNEL_HH_
#define _LOGSERVICECHANNEL_HH_
/**
 * @file   LogServiceChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines LogService channel
 *
 */

#include <boost/thread/mutex.hpp>
#include "Channel.hh"
#include "LogComponentBase.hh"
#include "LogORBMgr.hh"

namespace dadi {

/**
 * @class LogServiceChannel
 * @brief channel to log messages in LogService
 */
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
  /**
   * @brief destructor
   */
  ~LogServiceChannel();

  void open();
  void close();
  void log(const Message& msg);
private:
  // TODO: update LogComponentBase to be thread-safe ?
  LogComponentBase *lb; /**< LogComponentBase */
  boost::mutex mutex_; /**< mutext to protect concurrent access */
};

} /* namespace dadi */


#endif /* _LOGSERVICECHANNEL_HH_ */

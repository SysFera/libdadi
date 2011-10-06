#ifndef _NULLCHANNEL_HH_
#define _NULLCHANNEL_HH_
/**
 * @file   Logging/NullChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines channel that discards log messages
 * @section Licence
 *   |LICENCE|
 *
 */

#include "Channel.hh"

namespace dadi {

/**
 * @class NullChannel
 * @brief channel that discards log messages
 */
class NullChannel : public Channel {
public:
  /**
   * @brief constructor
   */
  NullChannel();
  /**
   * @brief destructor
   */
  ~NullChannel();

  /**
   * @brief log message
   * @param msg message to log
   */
  void
  log(const Message& msg);
};

} /* namespace dadi */

#endif /* _NULLCHANNEL_HH_ */

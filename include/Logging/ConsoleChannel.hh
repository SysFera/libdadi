#ifndef _CONSOLECHANNEL_HH_
#define _CONSOLECHANNEL_HH_
/**
 * @file   ConsoleChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines channel that logs into console
 *
 */

#include <iosfwd>
#include <boost/thread/mutex.hpp>
#include "Channel.hh"

namespace dadi {

class Message;

/**
 * @class ConsoleChannel
 * @brief channel that logs into console (std::cout, std::cerr)
 * or any C++ stream
 */
class ConsoleChannel : public Channel {
public:
  /**
   * @brief default constructor (by default: std::cout)
   */
  ConsoleChannel();
  /**
   * @brief constructor
   * @param out C++ stream
   */
  ConsoleChannel(std::ostream& out);
  void log(const Message& msg);
private:
  std::ostream& out_; /**< C++ stream */
  boost::mutex mutex_; /**< mutex protecting concurrent access */
};

} /* namespace dadi */

#endif /* _CONSOLECHANNEL_HH_ */

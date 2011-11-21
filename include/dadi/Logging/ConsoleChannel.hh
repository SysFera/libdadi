/**
 * @file   Logging/ConsoleChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines channel that logs into console
 * @section Licence
 *   |LICENCE|
 *
 */

#ifndef _CONSOLECHANNEL_HH_
#define _CONSOLECHANNEL_HH_

#include <iosfwd>
#include <boost/thread/mutex.hpp>
#include <dadi/Logging/Channel.hh>

namespace dadi {

class Message;

/**
 * @class ConsoleChannel
 * @brief channel that logs into console (std::cout, std::cerr)
 * or any C++ stream
 * It only write message followed by a newline (unix)
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
  explicit ConsoleChannel(std::ostream& out);
  void
  log(const Message& msg);
private:
  std::ostream& out_; /**< C++ stream */
  boost::mutex mutex_; /**< mutex protecting concurrent access */
};

} /* namespace dadi */

#endif /* _CONSOLECHANNEL_HH_ */

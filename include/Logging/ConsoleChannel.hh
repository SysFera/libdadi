#ifndef _CONSOLECHANNEL_HH_
#define _CONSOLECHANNEL_HH_

#include <iosfwd>
#include "Channel.hh"

namespace dadi {

class Message;

class ConsoleChannel : public Channel {
public:
  ConsoleChannel();
  ConsoleChannel(std::ostream& out);
  void log(const Message& msg);
private:
  std::ostream& out_;
};

} /* namespace dadi */

#endif /* _CONSOLECHANNEL_HH_ */

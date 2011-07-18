#include <Logging/ConsoleChannel.hh>
#include <iostream>
#include <Logging/Message.hh>

namespace dadi {

ConsoleChannel::ConsoleChannel() : out_(std::clog) {}

ConsoleChannel::ConsoleChannel(std::ostream& out) : out_(out) {}

void
ConsoleChannel::log(const Message& msg) {
  out_ << msg.getText() << "\n";
}

} /* namespace dadi */


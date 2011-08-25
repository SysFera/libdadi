/**
 * @file   ConsoleChannel.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  ConsoleChannel implementation
 * @section
 *   <licence>
 *
 */

#include <Logging/ConsoleChannel.hh>
#include <iostream>
#include <boost/thread/locks.hpp>
#include <Logging/Message.hh>

namespace dadi {

typedef boost::lock_guard<boost::mutex> Lock;
ConsoleChannel::ConsoleChannel() : out_(std::clog) {}

ConsoleChannel::ConsoleChannel(std::ostream& out) : out_(out) {}

void
ConsoleChannel::log(const Message& msg) {
  Lock lock(mutex_);

  out_ << msg.getText() << "\n";
}

} /* namespace dadi */


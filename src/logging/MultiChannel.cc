/**
 * @file   MultiChannel.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines special channel that redirect log messages
 * to multiple channels.
 * @section Licence
 *   |LICENCE|
 *
 */

#include <dadi/Logging/MultiChannel.hh>
#include <boost/thread/locks.hpp>
#include <dadi/Logging/Message.hh>

typedef boost::lock_guard<boost::mutex> Lock;

namespace dadi {

MultiChannel::MultiChannel() {}

MultiChannel::~MultiChannel() {}

void
MultiChannel::addChannel(ChannelPtr channel) {
  Lock lock(mutex_);

  if (channel) {
    channels_.push_back(channel);
  }
}

void
MultiChannel::removeChannel(ChannelPtr channel) {
  Lock lock(mutex_);

  channels_.remove(channel);
}

void
MultiChannel::log(const Message& msg) {
  Lock lock(mutex_);
  Channels::iterator it = channels_.begin();
  for (;it != channels_.end(); ++it) {
    (*it)->log(msg);
  }
}

void
MultiChannel::open() {}

void
MultiChannel::close() {}

int
MultiChannel::getCount() const {
  return channels_.size();
}

} /* namespace dadi */

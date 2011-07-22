#include <Logging/MultiChannel.hh>
#include <boost/thread/locks.hpp>
#include <Logging/Message.hh>

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
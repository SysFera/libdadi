#ifndef _CHANNEL_HH_
#define _CHANNEL_HH_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "Attributes.hh"

namespace dadi {

class Message;

class Channel : public dadi::Attributes {
public:
  Channel();
  virtual ~Channel();

  virtual void open();
  virtual void close();
  virtual void log(const Message& msg) = 0;
};

typedef boost::shared_ptr<Channel> ChannelPtr;

} /* namespace dadi */

#endif /* _CHANNEL_HH_ */

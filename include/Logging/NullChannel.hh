#ifndef _NULLCHANNEL_HH_
#define _NULLCHANNEL_HH_

#include "Channel.hh"

namespace dadi {

class NullChannel : public Channel {
public:
  NullChannel();
  ~NullChannel();

  void log(const Message& msg);
};

} /* namespace dadi */

#endif /* _NULLCHANNEL_HH_ */

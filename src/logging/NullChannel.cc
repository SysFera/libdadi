#include <Logging/NullChannel.hh>

namespace dadi {

NullChannel::NullChannel() {}

NullChannel::~NullChannel() {}

void
NullChannel::log(const Message& msg) {}

} /* namespace dadi */

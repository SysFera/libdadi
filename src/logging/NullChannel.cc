/**
 * @file   NullChannel.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines channel that discards log messages
 * @section License
 *   |LICENSE|
 *
 */

#include "dadi/Logging/NullChannel.hh"

namespace dadi {

NullChannel::NullChannel() {}

NullChannel::~NullChannel() {}

void
NullChannel::log(const Message& msg) {}

} /* namespace dadi */

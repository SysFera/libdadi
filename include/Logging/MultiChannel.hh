#ifndef _MULTICHANNEL_HH_
#define _MULTICHANNEL_HH_
/**
 * @file   Logging/MultiChannel.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines special channel that redirect log messages
 * to multiple channels.
 * @section Licence
 *   |LICENCE|
 *
 */

#include <list>
#include <boost/thread/mutex.hpp>
#include "Channel.hh"

namespace dadi {

class Message;

/**
 * @typedef Channels
 * @brief list of channels
 */
typedef std::list<ChannelPtr> Channels;

/**
 * @class MultiChannel
 * @brief redirects log messages to multiple channels
 */
class MultiChannel : public Channel {
public:
  /**
   * @brief constructor
   */
  MultiChannel();
  /**
   * @brief destructor
   */
  virtual ~MultiChannel();

  void open();
  void close();
  void log(const Message& msg);

  /**
   * @brief add a new channel
   * @param channel channel to be added
   */
  void addChannel(ChannelPtr channel);
  /**
   * @brief remove a channel
   * @param channel channel to be removed
   */
  void removeChannel(ChannelPtr channel);
  /**
   * @brief
   * @return number of channels registered
   */
  int getCount() const;
private:
  Channels channels_; /**< channels */
  boost::mutex mutex_; /**< mutex that protects concurrent access */
};


} /* namespace dadi */

#endif /* _MULTICHANNEL_HH_ */

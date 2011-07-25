#ifndef _MULTICHANNEL_HH_
#define _MULTICHANNEL_HH_

#include "Channel.hh"
#include <list>
#include <boost/thread/mutex.hpp>

namespace dadi {

class Message;

typedef std::list<ChannelPtr> Channels;

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
  Channels channels_;
  boost::mutex mutex_;
};


} /* namespace dadi */

#endif /* _MULTICHANNEL_HH_ */

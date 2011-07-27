#ifndef _CHANNEL_HH_
#define _CHANNEL_HH_
/**
 * @file Channel.hh
 * @brief defines an abstract base class for channels
 * @author: Haikel Guémar <haikel.guemar@sysfera.com>
 *
 */
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "Attributes.hh"

namespace dadi {

class Message;

/**
 * @class Channel
 * @brief channels abstract base class
 */
class Channel : public dadi::Attributes {
public:
  /**
   * @brief default constructor
   */
  Channel();
  /**
  * @brief destructor
  */
  virtual ~Channel();

  /**
   * @brief open channel for logs
   */
  virtual void open();
  /**
   * @brief close channel for logs
   */
  virtual void close();
  /**
   * @brief logs message
   * @param msg Message to be logged
   * @warning must be reimplemented
   */
  virtual void log(const Message& msg) = 0;
};

typedef boost::shared_ptr<Channel> ChannelPtr;

} /* namespace dadi */

#endif /* _CHANNEL_HH_ */

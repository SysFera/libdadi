#ifndef _CONFIG_HH_
#define _CONFIG_HH_
/**
 * @file   Config.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines configuration store
 *
 */

#include <map>
#include <string>

#include <boost/any.hpp>
#include <boost/thread.hpp>

#include "Singleton.hh"

namespace dadi {

  /**
   * @class Config
   * @brief thread-safe singleton configuration store
   */
  class Config : public Singleton<Config> {
    /**
     * @brief configuration is actually stored in a std::map
     */
    typedef std::map<std::string, boost::any> ConfigStore;
    friend class Singleton<Config>;
  public:
    /**
     * @brief writer subscript operator
     * @param key configuration key
     */
    boost::any& operator[](const std::string& key) {
      boost::unique_lock<boost::shared_mutex> lock(mutex_);
      return store_[key];
    }
    /**
     * @brief reader subscript operator
     * @param key configuration key
     */
    boost::any operator[](const std::string& key) const {
      boost::shared_lock<boost::shared_mutex> lock(mutex_);
      return store_[key];
    }

  private:
    /**
     *mutex protecting concurrent access
     */
    mutable boost::shared_mutex mutex_;
    mutable ConfigStore store_; /**< actual configuration store */
  };
}

#endif /* _CONFIG_HH_ */

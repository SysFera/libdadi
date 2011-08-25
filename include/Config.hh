#ifndef _CONFIG_HH_
#define _CONFIG_HH_
/**
 * @file   Config.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines configuration store
 * @section Licence
 *   |LICENCE|
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
   * @brief thread-safe singleton configuration store using the CRTP idiom
   */
  class Config : public Singleton<Config> {
    /**
     * @brief configuration is actually stored in a std::map
     */
    typedef std::map<std::string, boost::any> ConfigStore;
    typedef ConfigStore::size_type size_type;
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

    /**
     * @brief count element with a specific key
     * @param key configuration key
     * @return the number of element (0 or 1) having this specific key
     */
    size_type count(const std::string& key) const {
      boost::shared_lock<boost::shared_mutex> lock(mutex_);
      return store_.count(key);
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

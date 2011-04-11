#ifndef _CONFIG_HH_
#define _CONFIG_HH_

#include <map>
#include <string>

#include <boost/any.hpp>
#include <boost/thread.hpp>

#include "singleton.hh"

namespace diet {

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
    // writer subscript operator
    boost::any& operator[](const std::string& key) {
      boost::unique_lock<boost::shared_mutex> lock(mutex_);
      return store_[key];
    }
    // reader subscript operator
    boost::any operator[](const std::string& key) const {
      boost::shared_lock<boost::shared_mutex> lock(mutex_);
      return store_[key];
    }

  private:
    mutable boost::shared_mutex mutex_;
    mutable ConfigStore store_;
  };
}

#endif /* _CONFIG_HH_ */

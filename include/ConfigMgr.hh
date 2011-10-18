/**
 * @file   ConfigMgr.hh
 * @author Ibrahima Cissé <ibrahima.cisse@sysfera.com>
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines local configuration manager
 * @section Licence
 *  |LICENCE|
 *
 */

#ifndef _CONFIGMGR_HH_
#define _CONFIGMGR_HH_

#include <list>
#include <string>
#include <boost/any.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include "Config.hh"

namespace dadi {

/**
 * @class ConfigMgr
 * @brief A configuration manager class
 */
class ConfigMgr  {
public:
  /**
   * @ A constructor taking the config store
   * @param store
   */
  explicit ConfigMgr(dadi::ConfigStore& store);

  /**
   * @brief set the config root
   * @param key the config root key
   */
  void
  setRootKey(const std::string& key);


  /**
   * @brief get the config root
   */
  std::string
  getRootKey() const;


  /**
   * @brief Get a set of nodes sharing the same key inside a section
   * @param sectionName the section name
   * @param key nodes name
   */
  template<typename valueType>
  std::vector<valueType>
  get(const std::string& sectionName, const std::string& key) const {
    return proxyCache->template get<valueType>(sectionName, key);
  }

  /**
   * @brief get a final node
   * @param key the node name
   */
  template<typename valueType>
  valueType
  get(const std::string& key) const {
    return proxyCache->template get<valueType>(key);
  }


private:
  /**
   * @class nested class to hide the buffer
   */
  class ProxyCache {
  public:
    /**
     * @brief Constructor taking the store to hide
     * @param store the tree to hide
     */
    explicit ProxyCache(const dadi::ConfigStore& store);

    /**
     * @brief get a final node
     * @param key  node name
     */
    template<typename valueType>
    valueType
    get(const std::string& key) {
      valueType result;

      try {
        result = store_.get<valueType>(key);
      }
      catch (const boost::property_tree::ptree_bad_path& e) {
        BOOST_THROW_EXCEPTION(UnknownParameterError() << errinfo_msg(e.what()));
      } catch (const boost::property_tree::ptree_bad_data& e) {
        BOOST_THROW_EXCEPTION(InvalidParameterError() << errinfo_msg(e.what()));
      }

      return result;
    }

    /**
     * @brief get an intermediary node
     * @param key relative key
     */
    template<typename valueType>
    std::vector<valueType>
    get(const std::string& sectionName, const std::string& key) {
      std::vector<valueType> values;
      try {
        dadi::ConfigStore child_tree;
        BOOST_FOREACH(dadi::ConfigStore::value_type &ptree_enfant,
                      store_.get_child(sectionName)) {
          child_tree = ptree_enfant.second;
          values.push_back(child_tree.get<valueType>(key));
        }
      } catch (const boost::property_tree::ptree_bad_path& e) {
        BOOST_THROW_EXCEPTION(UnknownParameterError()
                              << errinfo_msg(e.what()));
      } catch (const boost::property_tree::ptree_bad_data& e) {
        BOOST_THROW_EXCEPTION(InvalidParameterError()
                              << errinfo_msg(e.what()));
      }
      return values;
    }

  private:
    dadi::ConfigStore store_;
  };

  boost::scoped_ptr<ProxyCache> proxyCache;  /**< actual configuration store */
  std::string rootKey;
};

/**
 * @brief factory method to build a valid partial config manager
 * @param rootKey a namespace  used to get the subtree in global config
 * @return A new config manager
 */
boost::shared_ptr<ConfigMgr>
getConfigMgr(const std::string& rootKey) {
  dadi::Config& config = dadi::Config::instance();
  // get the a subset of the global configuration
  ConfigStore subConf = config.get_child(rootKey);
  boost::shared_ptr<ConfigMgr> newConfig(new dadi::ConfigMgr(subConf));
  newConfig->setRootKey(rootKey);
  return newConfig;
}
}

#endif /* _CONFIGMGR_HH_ */

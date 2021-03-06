/**
 * @file   Registry.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief defines Registry containing plugins
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _REGISTRY_HH_
#define _REGISTRY_HH_

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "IPlugin.hh"
#include "Loader.hh"
#include "PluginInfo.hh"
#include "Singleton.hh"
#include "Exception/Plugins.hh"

namespace dadi {
// plugin info lookup structure
/**
 * @brief pinfo_set name key
 */
struct name {};
/**
 * @brief pinfo_set interface key
 */
struct interface {};

// key extractors
/**
 * @struct name_extractor
 * @brief pinfo_set name extractor
 */
struct name_extractor {
  typedef std::string result_type; /**< extracted data type */
  /**
   * @brief effective extractor
   * @return extracted member
   */
  const result_type&
  operator()(PluginInfoPtr pi) const { return pi->name; }
};

/**
 * @struct interface_extractor
 * @brief pinfo_set interface extractor
 */
struct interface_extractor {
  typedef std::string result_type; /**< extracted data type */
  /**
   * @brief effective extractor
   * @return extracted member
   */
  const result_type&
  operator()(PluginInfoPtr pi) const {
    return pi->interface;
  }
};


/**
 * @typedef pinfo_set
 * @brief container of PluginInfoPtr indexed on:
 * - name (unique ordered index)
 * - interface (ordered index)
 */
typedef boost::multi_index_container<
  PluginInfoPtr,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<
      boost::multi_index::tag<name>,
      name_extractor>,
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag<interface>,
      interface_extractor> >
  > pinfo_set;

/**
 * @typedef pinfo_set_by_name
 * @brief pinfo_set index on name
 */
typedef pinfo_set::index<name>::type pinfo_set_by_name;
/**
 * @typedef pinfo_set_by_interface
 * @brief pinfo_set index on interface
 */
typedef pinfo_set::index<interface>::type pinfo_set_by_interface;

/**
 * @class Registry
 * @brief hides (un)loading plugins and creation of instances from user
 */
class Registry : public dadi::Singleton<Registry> {
public:

  friend class dadi::Singleton<Registry>;
  /**
   * @brief register a plugin into registry
   * @param pInfo
   */
  void
  registerPlugin(PluginInfoPtr pInfo);

  // unregister(const std::string& pName);
  /**
   * @brief add new search path for plugin manifests
   * @param path
   */
  void
  addPath(const std::string& path);

  /**
   * @brief remove search path for plugin manifests
   * @param path
   */
  void
  removePath(const std::string& path);

  /**
   * @brief load plugins
   */
  void
  load();
  /**
   * @brief list plugins implementing an interface
   * @param pName interface name
   * @return std::list
   */
  std::list<std::string>
  listPluginsByInterface(const std::string& pName);

  /**
   * @brief get manifests search paths
   * @return search paths
   */
  std::list<std::string>
  paths() {
    return paths_;
  }

  /**
   * @brief get a plugin instance from its name
   * @param pName plugin name
   * @return instance (or NULL)
   */
  template<typename Plugin>
  Plugin *
  getByName(const std::string& pName) {
    void *factory(NULL);
    Plugin *instance(NULL);

    /* Boost::Multi-Index has a bug as it returns a valid index
       even when container is empty
    */
    if (cache_.empty()) {
      return static_cast<Plugin *>(instance);
    }

    pinfo_set_by_name& index = cache_.get<name>();
    pinfo_set_by_name::iterator it = index.find(pName);

    if (index.end() == it) {
      BOOST_THROW_EXCEPTION(
        dadi::PluginError()
        << dadi::errinfo_msg("can not find: "+ pName));
    }

    factory = (*it)->factory;
    if (factory) {
      reinterpret_cast<factory_function>(factory)(reinterpret_cast<void**>(&instance));
      instance->plugin_info(*it);
    }

    SharedLibraryPtr sPtr = (*it)->sPtr;

    if (sPtr) {
      std::list<std::string>& deps = (*it)->deps;
      std::list<std::string>::iterator ita = deps.begin();

      IPlugin *p = NULL;
      for (; ita != deps.end(); ++ita) {
        p = getByInterface<IPlugin>(*ita);
      }
    }

    return static_cast <Plugin *>(instance);
  }

  /**
   * @brief get a plugin instance from its interface
   * @param pName interface name
   * @return instance (or NULL)
   */
  template<typename Interface>
  Interface *
  getByInterface(const std::string& pName) {
    void *factory(NULL);
    Interface *instance(NULL);

    if (cache_.empty()) {
      return static_cast<Interface *>(instance);
    }

    pinfo_set_by_interface& index = cache_.get<interface>();
    pinfo_set_by_interface::iterator it = index.find(pName);

    if (index.end() == it) {
      BOOST_THROW_EXCEPTION(
        dadi::PluginError()
        << dadi::errinfo_msg("can not find: "+ pName));
    }

    factory = (*it)->factory;
    if (factory) {
      reinterpret_cast<factory_function>(factory)(reinterpret_cast<void**>(&instance));
      instance->plugin_info(*it);
    }

    std::cout << (*it)->name << "\n";
    SharedLibraryPtr sPtr = (*it)->sPtr;
    if (sPtr) {
      std::list<std::string>& deps = (*it)->deps;
      std::list<std::string>::iterator ita = deps.begin();

      for (; ita != deps.end(); ++ita) {
        std::cout << *ita << "\n";
      }
    }
    return static_cast<Interface *>(instance);
  }

private:

  /**
   * @brief constructor
   * @return
   */
  Registry();

  std::list<std::string> paths_; /**< search paths for plugins manifests */
  boost::shared_ptr<Loader> loader_; /**< loader */
  pinfo_set cache_; /**< Registry cache */
};

} /* namespace dadi */

#endif  /* _REGISTRY_HH_ */

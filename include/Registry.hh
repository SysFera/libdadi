#ifndef _REGISTRY_HH_
#define _REGISTRY_HH_
/**
 * @file   Registry.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief defines Registry containing plugins
 * @section
 *   <licence>
 *
 */


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

#include "IPlugin.hh"
#include "Loader.hh"
#include "PluginInfo.hh"
#include "Exception/Plugins.hh"

namespace dadi {
// plugin info lookup structure
struct name {};
struct interface {};

// key extractors
struct name_extractor {
  typedef std::string result_type;
  const result_type& operator()(PluginInfoPtr pi) const { return pi->name; }
};

struct interface_extractor {
  typedef std::string result_type;
  const result_type& operator()(PluginInfoPtr pi) const {
    return pi->interface;
  }
};


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

typedef pinfo_set::index<name>::type pinfo_set_by_name;
typedef pinfo_set::index<interface>::type pinfo_set_by_interface;

/**
 * @class Registry
 * @brief hides (un)loading plugins and creation of instances from user
 */
class Registry : public boost::noncopyable {
public:
  /**
   * @brief constructor
   * @return
   */
  Registry();
  /**
   * @brief register a plugin into registry
   * @param pInfo
   */
  void registerPlugin(PluginInfoPtr pInfo);

  // unregister(const std::string& pName);
  /**
   * @brief add new search path for plugin manifests
   * @param path
   */
  void addPath(const std::string& path);

  void load();
  /**
   * @brief get manifests search paths
   * @return search paths
   */
  std::list<std::string> paths() {
    return paths_;
  }

  /**
   * @brief get a plugin instance from its name
   * @param pName plugin name
   * @return instance (or NULL)
   */
  template<typename Plugin>
  Plugin *getByName(const std::string& pName) {
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
      ((bool (*)(void **))(factory))((void**) &instance);
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
//    void *conn = sPtr->symbol("connect_IPerson_IWork");

    return static_cast <Plugin *>(instance);
  }

  template<typename Interface>
  Interface *getByInterface(const std::string& pName) {
    void *factory(NULL);
    Interface *instance(NULL);

    if (cache_.empty()) {
      return static_cast<Interface *>(instance);
    }

    pinfo_set_by_interface& index = cache_.get<interface>();
    pinfo_set_by_interface::iterator it = index.find(pName);

    if (index.end() == it){

      BOOST_THROW_EXCEPTION(
           dadi::PluginError()
                 << dadi::errinfo_msg("can not find: "+ pName));
    }

    factory = (*it)->factory;
    if (factory) {
      ((int (*)(void **))(factory))((void**) &instance);
    }

    std::cout << (*it)->name << "\n";
    SharedLibraryPtr sPtr = (*it)->sPtr;
    if (sPtr) {
      std::list<std::string>& deps = (*it)->deps;
      std::list<std::string>::iterator ita = deps.begin();

      for (; ita != deps.end(); ++ita) {
        std::cout << *ita << "\n";
        // IPlugin *p = get<IPlugin>(*ita);
      }
    }
    return static_cast<Interface *>(instance);
  }

private:
  std::list<std::string> paths_; /**< search paths for plugins manifests */
  boost::scoped_ptr<Loader> loader_; /**< loader */
  pinfo_set cache_; /**< Registry cache */
};

} /* namespace dadi */

#endif /* _REGISTRY_HH_ */

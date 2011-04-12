// -*- mode: c++ -*-
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

#include "IPlugin.hh"
#include "Loader.hh"
#include "PluginInfo.hh"

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
  const result_type& operator()(PluginInfoPtr pi) const { return pi->interface; }
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

class Registry : public boost::noncopyable {
public:
  Registry();

  void registerPlugin(PluginInfoPtr pInfo);

  // unregister(const std::string& pName);

  void addPath(const std::string& path);

  void load();

  std::list<std::string> paths() {
    return paths_;
  }

  template<typename Plugin>
  Plugin *getByName(const std::string& pName) {
    void *factory(NULL);
    Plugin *instance(NULL);

    pinfo_set_by_name& index = cache_.get<name>();
    pinfo_set_by_name::iterator it = index.find(pName);

    if (index.end() != it) {
      factory = (*it)->factory;
      if (factory) {
        ((bool (*)(void **))(factory))((void**) &instance);
      }
    }

    SharedLibraryPtr sPtr = (*it)->sPtr;
    std::list<std::string>& deps = (*it)->deps;
    std::list<std::string>::iterator ita = deps.begin();

    IPlugin *p = NULL;
    for(; ita != deps.end(); ++ita) {
      p = getByInterface<IPlugin>(*ita);
    }

//    void *conn = sPtr->symbol("connect_IPerson_IWork");

    return static_cast <Plugin *>(instance);
  }

  template<typename Interface>
  Interface *getByInterface(const std::string& pName) {
    void *factory(NULL);
    Interface *instance(NULL);

    pinfo_set_by_interface& index = cache_.get<interface>();
    pinfo_set_by_interface::iterator it = index.find(pName);

    if (index.end() != it) {
      factory = (*it)->factory;
      if (factory) {
        ((bool (*)(void **))(factory))((void**) &instance);
      }
    }

    SharedLibraryPtr sPtr = (*it)->sPtr;
    std::list<std::string>& deps = (*it)->deps;
    std::list<std::string>::iterator ita = deps.begin();

    for(; ita != deps.end(); ++ita) {
      std::cout << *ita << "\n";
      //IPlugin *p = get<IPlugin>(*ita);
    }

    return static_cast <Interface *>(instance);
  }

private:
  std::list<std::string> paths_;
  boost::scoped_ptr<Loader> loader_;
  pinfo_set cache_;
};


#endif /* _REGISTRY_HH_ */

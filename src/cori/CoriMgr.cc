/**
 * @file   CoriMgr.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  dadi Collector of resources information
 * @section License
 *   |LICENSE|
 *
 */

#include "dadi/CoriMgr.hh"
#include <boost/foreach.hpp>
#include "dadi/dadi-config.hh"
#include "dadi/Registry.hh"

namespace dadi {

CoriMgr::CoriMgr(bool loadDefault) {
  dadi::Registry& reg = dadi::Registry::instance();
  if (loadDefault) {
    const std::list<std::string>& paths = reg.paths();
    std::list<std::string>::const_iterator it =
    std::find(paths.begin(), paths.end(), CORI_PLUGINS_SYSTEM_DEFAULT_PATH);
    if (it == paths.end()) {
      reg.addPath(CORI_PLUGINS_SYSTEM_DEFAULT_PATH);
    }
    reg.load();
  }

  std::list<std::string> names = reg.listPluginsByInterface("ICori");
  BOOST_FOREACH(const std::string& name, names) {
    ICori* p = reg.getByName<ICori>(name);
    p->init();
    plugins.push_back(p);
  }
}

CoriMgr::~CoriMgr() {}

std::list<std::string>
CoriMgr::listPlugins() {
  std::list<std::string> l;
  BOOST_FOREACH(ICori *p, plugins) {
    PluginInfoPtr piPtr = p->plugin_info();
    l.push_back(piPtr->name);
  }
  return l;
}


Attributes
CoriMgr::listMetrics() {
  Attributes a;
  BOOST_FOREACH(ICori *p, plugins) {
    Attributes b = p->listMetrics();
    a.merge(b);
  }

  return a;
}

Attributes
CoriMgr::getMetrics(const std::string& filter) {
  const std::string& filter_ =
  !filter.empty() ? filter : dadi::str(listMetrics());

  Attributes a;
  BOOST_FOREACH(ICori *p, plugins) {
    Attributes b = p->getMetrics(filter_);
    a.merge(b);
  }

  return a;
}

} /* namespace dadi */

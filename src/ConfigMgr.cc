/**
 * @file   ConfigMgr.cc
 * @author Benjamin Depardon <benjamin.depardon@sysfera.com>
 * @brief  local configuration manager implementation
 * @section Licence
 *  |LICENCE|
 *
 */


#include "ConfigMgr.hh"

namespace dadi {

ConfigMgr::ConfigMgr(dadi::ConfigStore& store)
  : proxyCache(new ProxyCache(store)), rootKey("undefined") {}


void
ConfigMgr::setRootKey(const std::string& key) {
  rootKey = key;
}

std::string
ConfigMgr::getRootKey() const {
  return rootKey;
}


ConfigMgr::ProxyCache::ProxyCache(const dadi::ConfigStore& store)
  : store_(store) {}

} /* namespace dadi */

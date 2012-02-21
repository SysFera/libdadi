/**
 * @file   Registry.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  dadi registry implementation
 * @section Licence
 *   |LICENCE|
 *
 */

#include "dadi/Registry.hh"

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

namespace dadi {

Registry::Registry() : loader_(new Loader(this)) {}

void
Registry::registerPlugin(PluginInfoPtr pInfo) {
  cache_.insert(pInfo);

  std::cerr << boost::format("[REGISTER]: factory %1% registered\n")
    % pInfo->name;
}

void
Registry::addPath(const std::string& path) {
  paths_.push_back(path);
}

void
Registry::load() {
  using boost::filesystem::directory_iterator;
  using boost::filesystem::exists;
  using boost::filesystem::path;

  // loop over all registred directories
  BOOST_FOREACH(std::string d, paths_) {
    // Jump to the next if the current directory does not exist
    if (exists(path(d))) {
      directory_iterator p(d);
      for (; p != directory_iterator(); ++p) {
        if (((p->path()).extension()) == ".xml") {
          loader_->loadPlugin(p->path().string().c_str());
        }
      }
    }
  }
}

std::list<std::string>
Registry::listPluginsByInterface(const std::string& pName) {
  std::list<std::string> plugins;
  if (!cache_.empty()) {
    pinfo_set_by_interface& index = cache_.get<interface>();
    pinfo_set_by_interface::iterator lb = index.lower_bound(pName);
    pinfo_set_by_interface::iterator ub = index.upper_bound(pName);

    for (pinfo_set_by_interface::iterator it = lb;
         it != ub; ++it) {
      plugins.push_back((*it)->name);
    }
  }

  return plugins;
}


} /* namespace dadi */


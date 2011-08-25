/**
 * @file   Registry.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  dadi registry implementation
 *
 */

#include "Registry.hh"

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
  using namespace boost::filesystem;
  // loop over all registred directories
  BOOST_FOREACH(std::string d, paths_) {
    // Jump to the next if the current directory does not exist
    if(exists(path(d)) ){
      directory_iterator p(d);
      for (; p != directory_iterator(); ++p) {
        if (((p->path()).extension()) == ".xml") {
          loader_->loadPlugin(p->path().c_str());
        }
      }
    }
  }
}

} /* namespace dadi */


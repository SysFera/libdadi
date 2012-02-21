/**
 * @file   Loader.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  plugin loader
 * @section Licence
 *   |LICENCE|
 *
 */

#include "dadi/Loader.hh"

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "dadi/IPlugin.hh"
#include "dadi/Registry.hh"
#include "dadi/Exception/Plugins.hh"

namespace dadi {

Loader::Loader() {}

Loader::Loader(Registry* reg) { reg_ = reg; }

Loader::~Loader() {}

std::string
Loader::findLibrary(std::string lib) const {
  using boost::filesystem::directory_iterator;

  std::list<std::string> l = reg_->paths();
  std::list<std::string>::iterator it = l.begin();

  for (;it != l.end(); ++it) {
    directory_iterator p(*it);
    for (; p!= directory_iterator(); ++p) {
      if (lib == (p->path()).filename()) {
        return p->path().string();
      }
    }
  }

  return "";
}


void
Loader::loadPlugin(const char *mFile) {
  PluginInfoPtr pInfo(new PluginInfo);
  parseConfig(mFile, pInfo);

  BOOST_FOREACH(std::string& v, pInfo->paths) {
    reg_->addPath(v);
  }

  std::string libname = SharedLibrary::prefix() +
    pInfo->name +
    SharedLibrary::suffix();
  std::cout << "library: " << libname << "\n";
  std::string libpath = findLibrary(libname);

  // load library
  SharedLibraryPtr sPtr(new SharedLibrary(libpath));
  if (!sPtr->isLoaded()) {
    std::cerr << boost::format("ERROR: can't load module %1%\n") % mFile;
    return;
  }

  // get factory
  pInfo->factory = sPtr->symbol("create_plugin_instance");
  if (!pInfo->factory) {
    std::cerr <<
      boost::format("ERROR: can't load symbol create_plugin_instance\n");
    return;
  }

  BOOST_FOREACH(std::string& dep, pInfo->deps) {
    std::cout << boost::format("dependency: %1%\n") % dep;
  }

  pInfo->sPtr = sPtr;
  reg_->registerPlugin(pInfo);
}

void
Loader::setRegistry(Registry *reg) {
  reg_ = reg;
}

// TODO: should throw an exception
void
Loader::parseConfig(const char *mFile, PluginInfoPtr pInfo) {
  using boost::property_tree::ptree;
  using boost::property_tree::ptree_bad_path;
  using boost::property_tree::xml_parser_error;

  ptree pt;

  try {
    read_xml(mFile, pt);

    pInfo->name = pt.get<std::string>("plugindescriptor.name");
    pInfo->version = pt.get<std::string>("plugindescriptor.version");
    pInfo->interface = pt.get<std::string>("plugindescriptor.interface");
    boost::optional<ptree&> metadataTree =
      pt.get_child_optional("plugindescriptor.metadata");

    if (metadataTree) {
      std::stringstream metadataStr;
      write_xml(metadataStr, metadataTree.get());

      (pInfo->metadata).loadAttr(metadataStr.str());
    }

    try {
      BOOST_FOREACH(ptree::value_type& v,
                    pt.get_child("plugindescriptor.paths")) {
        pInfo->paths.push_back(v.second.data());
      }
    } catch (const ptree_bad_path& e) {
    }


    // try {
    //   BOOST_FOREACH( ptree::value_type& v
    //                  pt.get_child("plugindescriptor.dependencies"))
    //     pInfo->deps.push_back(
    //       v.second.get<std::string>("<xmlattr>.interface"));
    // } catch (const ptree_bad_path& e) {
    // }
  } catch (const xml_parser_error& e) {
    std::cerr << e.what();
  }
}

} /* namespace dadi */

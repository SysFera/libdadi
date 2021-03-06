/**
 * @file PluginInfo.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.fr>
 * @brief plugin metadata holder class
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _PLUGININFO_HH_
#define _PLUGININFO_HH_

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include "dadi/SharedLibrary.hh"
#include "dadi/Attributes.hh"

namespace dadi {

/**
 * @class PluginInfo
 * @brief stores plugin metadata
 */
struct PluginInfo {
  /**
   * @brief default constructor
   */
  PluginInfo();

  /**
   * @brief copy constructor
   * @param other PluginInfo to copy
   */
  PluginInfo(const PluginInfo& other);

  /**
   * @brief assignment operator
   * @param other PluginInfo to copy
   */
  PluginInfo&
  operator=(const PluginInfo& other);

  /**
   * @brief equal to operator
   * @param other PluginInfo to compare
   */
  bool
  operator==(const PluginInfo& other);

  /**
   * @brief not equal to operator
   * @param other PluginInfo to compare
   */
  bool
  operator!=(const PluginInfo& other);

  /**
   * @brief test if plugin has been correctly loaded
   * @return
   */
  operator bool();

  boost::uuids::uuid uuid; /**< plugin uuid */
  std::string name; /**< plugin name */
  std::string version; /**< plugin version */
  std::string interface; /**< plugin interface */
  std::string path; /**< library path */
  Attributes metadata; /**< metadata */
  void *factory; /**< pointer to factory method */
  SharedLibraryPtr sPtr; /**< pointer to sharedLibrary */
  std::list<std::string> deps; /**< list of dependencies */
  std::list<std::string> paths; /**< path hints to search libraries */
};

/**
 * @typedef PluginInfoPtr
 * @brief shared pointer on PluginInfo
 */
typedef boost::shared_ptr<PluginInfo> PluginInfoPtr;

} /* namespace dadi */

#endif  /* _PLUGININFO_HH_ */

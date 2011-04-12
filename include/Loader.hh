// -*- mode: c++ -*-
#ifndef _LOADER_HH_
#define _LOADER_HH_

#include <list>
#include "PluginInfo.hh"
#include "SharedLibrary.hh"

namespace dadi {
class Registry;
class PluginInfo;
typedef std::list<SharedLibraryPtr> LibraryCache;

/**
 * @brief Load plugins and register them in the registry
 */
class Loader {
public:
  Loader();
  explicit Loader(Registry* reg);
  ~Loader();

  /**
   * @brief load a plugin
   * @param mFile plugin manifest
   */
  void loadPlugin(const char *mFile);
  /**
   * @brief set registry
   * @param reg plugin registry
   */
  void setRegistry(Registry *reg);
private:
  /**
   * @brief find library path in paths
   * @param lib name of the library
   * @return
   */
  std::string findLibrary(std::string lib) const;
  /**
   * @brief parse plugin manifest and load information
   * @param mFile plugin manifest to parse
   * @param pInfo PluginInfoPtr to set
   */
  void parseConfig(const char *mFile, PluginInfoPtr pInfo);

  Registry *reg_; /**< plugin registry */
};

} /* namespace dadi */
#endif /* _LOADER_HH_ */

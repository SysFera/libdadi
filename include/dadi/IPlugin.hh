/**
 * @file   IPlugin.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines plugin abstract base class
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _IPLUGIN_HH_
#define _IPLUGIN_HH_

#include "PluginInfo.hh"

/**
 * @example simple-plugin/IPerson.hh
 * This is a simple example to use the IPlugin class.
 *
 * @example simple-plugin/PersonA.cc
 * Implementation of an IPerson
 */


/** return code when plugin has been correctly loaded */
const int PLUGIN_OK = 0;

// win32 peculiarity required to export symbols in shared libraries
#if defined(WIN32)
#  if defined(DADI_EXPORTS)
#    define DADI_EXPORT __declspec(dllexport)
#  else /* DADI_EXPORTS */
#    define DADI_EXPORT __declspec(dllimport)
#  endif /* DADI_EXPORTS */
#else /* WIN32 */
#  define DADI_EXPORT
#endif /* WIN32 */

/**
 * @class IPlugin
 * @brief plugin abstract base clase
 */
class DADI_EXPORT IPlugin {
public:
  /**
   * @brief destructor
   */
  virtual ~IPlugin() {}
  /**
   * @brief initialize plugin
   */
  void
  init() {
    do_init();
  }
  /**
   * @brief set PluginInfo
   * @param piPtr PluginInfoPtr
   */
  void
  plugin_info(const dadi::PluginInfoPtr& piPtr) {
    piPtr_ = piPtr;
  }
  /**
   * @brief get PluginInfo
   * @return PluginInfoPtr
   */
  dadi::PluginInfoPtr&
  plugin_info() { return piPtr_; }

protected:
  /**
   * @brief implementation of init
   * @warning must be reimplemented
   */
  virtual void
  do_init() = 0;

  dadi::PluginInfoPtr piPtr_; /**< PluginInfoPtr */
};


extern "C" {
  /** factory method signature */
  typedef bool (*factory_function)(void **instance);

/**
 * @brief Plugin factory
 * Plugin writers must implement this function
 * it should instantiate a new IPlugin instance and return true in case
 * of success, false otherwise.
 * @param[out] instance
 */
  int DADI_EXPORT
  create_plugin_instance(void **instance);
}

#endif  /* _IPLUGIN_HH_ */

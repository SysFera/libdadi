// -*- mode: c++ -*-
#ifndef _IPLUGIN_HH_
#define _IPLUGIN_HH_

#include "PluginInfo.hh"

const int PLUGIN_OK = 0;

// win32 peculiarity required to export symbols in shared libraries
#if defined(WIN32)
#  if defined(DADI_EXPORTS)
#    define DADI_EXPORT __declspec( dllexport )
#  else /* DADI_EXPORTS */
#    define DADI_EXPORT __declspec( dllimport )
#  endif /* DADI_EXPORTS */
#else /* WIN32 */
#  define DADI_EXPORT
#endif /* WIN32 */

class DADI_EXPORT IPlugin {
public:
  virtual ~IPlugin() {}
  void init() { do_init(); }
  void plugin_info(dadi::PluginInfoPtr& piPtr) { piPtr_ = piPtr; }
  dadi::PluginInfoPtr& plugin_info() { return piPtr_; }
protected:
  virtual void do_init()=0;
  dadi::PluginInfoPtr piPtr_;
};


extern "C" {
  typedef bool (*factory_function)(void **instance);

/**
 * @brief Plugin factory
 * Plugin writers must implement this function
 * it should instantiate a new IPlugin instance and return true in case
 * of success, false otherwise.
 */
  int DADI_EXPORT create_plugin_instance(void **instance);
}

#endif /* _IPLUGIN_HH_ */

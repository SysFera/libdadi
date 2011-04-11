// -*- mode: c++ -*-
#ifndef _IPLUGIN_HH_
#define _IPLUGIN_HH_

#include "PluginInfo.hh"

// win32 peculiarity required to export symbols in shared libraries
#if defined(WIN32)
#  if defined(DIET_EXPORTS)
#    define DIET_EXPORT __declspec( dllexport )
#  else /* DIET_EXPORTS */
#    define DIET_EXPORT __declspec( dllimport )
#  endif /* DIET_EXPORTS */
#else /* WIN32 */
#  define DIET_EXPORT
#endif /* WIN32 */

class DIET_EXPORT IPlugin {
public:
  virtual ~IPlugin() {}
  void init() { do_init(); }
  void plugin_info(PluginInfoPtr& piPtr) { piPtr_ = piPtr; }
  PluginInfoPtr& plugin_info() { return piPtr_; }
protected:
  virtual void do_init()=0;
  PluginInfoPtr piPtr_;
};


extern "C" {
  typedef bool (*factory_function)(void **instance);

/**
 * @brief Plugin factory
 * Plugin writers must implement this function
 * it should instantiate a new IPlugin instance and return true in case
 * of success, false otherwise.
 */
  bool DIET_EXPORT create_plugin_instance(void **instance);
}


/* TODO: handle separately dependencies to an interface or a plugin */
#define DECLARE_COMP_SLOT(__ptype,__type,__name)                        \
  extern "C"                                                            \
  bool DIET_EXPORT connect_##__ptype##_##__type(void *bar, void *foo);  \

#define COMP_SLOT(__ptype,__type,__name)                              \
  public:                                                         \
  virtual void do_connect_##__name(__type * __foo) {                  \
    __name = __foo;                                                   \
  }                                                                   \
                                                                      \
  friend bool connect_##__ptype##_##__type(void *bar,void * foo);     \
protected:                                                            \
__type* __name;

#define IMPLEMENT_COMP_SLOT(__ptype,__type,__name)                      \
  bool connect_##__ptype##_##__type(void *bar, void *foo) {             \
    ((__ptype *)bar)->do_connect_##__name((__type *)foo);               \
    return true;                                                        \
  }                                                                     \

#endif /* _IPLUGIN_HH_ */

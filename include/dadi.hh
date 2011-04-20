#ifndef DADI_HH
#define DADI_HH

#define DADI_VERSION_MAJOR 0
#define DADI_VERSION_MINOR 9
#define DADI_VERSION_PATCH 0

/* The Plugin interface definition (dynamic module loading). */
#include "dadi/IPlugin.hh"
/* The Loader class for definition (dynamic module loading). */
#include "dadi/Loader.hh"
/* The PluginInfo class. (dynamic module loading). */
#include "dadi/PluginInfo.hh"
/* Registry class (dynamic module loading). */
#include "dadi/Registry.hh"

/* A thread safe singleton class (utility). */
#include "dadi/singleton.hh"

/* Thread safe classes to store application config (utility). */
#include "dadi/config.hh"
#include "dadi/options.hh"

#endif

#ifndef DADI_HH
#define DADI_HH

/**
 * @file dadi.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.fr>
 * @author Gael Le Mahec <gael.le.mahec@u-picardie.fr>
 * @brief convenience header
 * @section Licence
 *   |LICENCE|
 *
 */

#include "dadi/config-in.hh"

/* The Plugin interface definition (dynamic module loading). */
#include "dadi/IPlugin.hh"
/* The Loader class for definition (dynamic module loading). */
#include "dadi/Loader.hh"
/* The PluginInfo class. (dynamic module loading). */
#include "dadi/PluginInfo.hh"
/* Registry class (dynamic module loading). */
#include "dadi/Registry.hh"
/* A thread safe singleton class (utility). */
#include "dadi/Singleton.hh"
/* Thread safe classes to store application config (utility). */
#include "dadi/Config.hh"
/* Command line and configuration files parser */
#include "dadi/Options.hh"
/* logging framework */
#include "dadi/Logging.hh"


/**
 * @mainpage Libdadi
 *
 *
 * @section intro_sec Introduction
 *
 * Libdadi is a C++ library providing basic application building blocks such as:
 * - command-line parsing
 * - configuration file parsing
 * - attributes (aka metadata)
 * - log4j style loggers
 * - authentification framework
 *
 */

#endif

#ifndef _PLUGINS_ERROR_HH_
#define _PLUGINS_ERROR_HH_
/**
 * @file   Plugins.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  libdadi plugins exceptions classes
 * @section Licence
 *   |LICENCE|
 *
 */

#include "Base.hh"

namespace dadi {

/**
 * @class PluginError
 * @brief Plugins exception base class
 */
class PluginError : public Error {};

/**
 * @class UnknownPluginError
 * @brief report an unknown plugin
 */
class UnknownPluginError : public PluginError {};

/**
 * @class InvalidPluginError
 * @brief report an invalid plugin (bad type)
 */
class InvalidPluginError : public PluginError {};


} /* namespace dadi */

#endif /* _PLUGINS_ERROR_HH_ */

/**
 * @file   Exception/Parameters.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  libdadi parameters exceptions classes
 * @section Licence
 *   |LICENCE|
 *
 */

#ifndef _PARAMETERS_ERROR_HH_
#define _PARAMETERS_ERROR_HH_

#include "Base.hh"

namespace dadi {

/**
 * @class ParameterError
 * @brief Parameters exception base class
 */
class ParameterError : public Error {};

/**
 * @class UnknownParameterError
 * @brief report an unknown parameter
 */
class UnknownParameterError : public ParameterError {};

/**
 * @class InvalidParameterError
 * @brief report an invalid parameter (bad type)
 */
class InvalidParameterError : public ParameterError {};

} /* namespace dadi */

#endif /* _PARAMETERS_ERROR_HH_ */

/**
 * @file   Exception/Attributes.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  attributes exception classes
 * @section Licence
 *   |LICENCE|
 *
 */

#ifndef _ATTRIBUTES_ERROR_HH_
#define _ATTRIBUTES_ERROR_HH_

#include "dadi/Exception/Base.hh"

namespace dadi {

/**
 * @class AttributeError
 * @brief Attributes exception base class
 */
class AttributeError : public Error {};

/**
 * @class UnknownAttributeError
 * @brief report an unknown attribute
 */
class UnknownAttributeError : public AttributeError {};

/**
 * @class InvalidAttributeError
 * @brief report an invalid attribute (bad type)
 */
class InvalidAttributeError : public AttributeError {};

/**
 * @class ParsingAttributeError
 * @brief report an invalid attribute (bad parsing)
 */
class ParsingAttributeError : public AttributeError {};

} /* namespace dadi */

#endif /* _ATTRIBUTES_ERROR_HH_ */

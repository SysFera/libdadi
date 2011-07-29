#ifndef _ATTRIBUTES_ERROR_HH_
#define _ATTRIBUTES_ERROR_HH_

#include "Base.hh"

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


} /* namespace dadi */

#endif /* _ATTRIBUTES_ERROR_HH_ */

/**
 * @file   Exception/Base.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  libdadi root exceptions classes
 * @section Licence
 *   |LICENCE|
 *
 */

#ifndef _BASE_HH_
#define _BASE_HH_

#include <exception>
#include <string>
#include <boost/exception/all.hpp>

namespace dadi {

/**
 * @class Error
 * @brief root exception class
 * all exception classes in libdadi **must** derive from this one
 */
class Error : virtual public std::exception,
              virtual public boost::exception {};

/**
 * @class NotImplementedError
 * @brief non-implemented feature
 */
class NotImplementedError : public Error {};

/**
 * @class NotAvailableException
 * @brief this feature is not available
 */
class NotAvailableException : public Error {};

/** generic error message */
typedef boost::error_info<struct tag_errinfo_msg, std::string> errinfo_msg;

} /* namespace dadi */


#endif /* _BASE_HH_ */

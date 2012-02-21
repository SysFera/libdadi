#ifndef _PARSERS_HH_
#define _PARSERS_HH_
/**
 * @file   detail/Parsers.hh
 * @author Ibrahima Cissé <ibrahima.cisse@sysfera.com>
 * @author Haïkel Guémar <haikel.guemar@gmail.com>
 * @brief  common utils for data serialization
 * @section Licence
 *  |LICENCE|
 *
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "dadi/Exception/Parameters.hh"

namespace dadi {

/**
 * @brief allowed formats (xml, ini, json, info)
 */
enum Format {
  FORMAT_XML = 0,
  FORMAT_INI,
  FORMAT_JSON,
  FORMAT_INFO
};

/**
 * @typedef ConfigStore
 * @brief Configuration tree backend (currently boost::property_tree)
 */
typedef boost::property_tree::ptree ConfigStore;

} /* namespace dadi */

#endif /* _PARSERS_HH_ */

/**
 * @file   ICori.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  define Cori plugin base class
 * @section Licence
 *   |LICENCE|
 *
 */


#ifndef _ICORI_HH_
#define _ICORI_HH_

#include <string>
#include "dadi/IPlugin.hh"
#include "dadi/Attributes.hh"

/**
 * @example simple-cori/main.cc
 * this is a simple example on how to use the CORI framework
 *
 */


namespace dadi {

/**
 * @class ICori
 * @brief abstract interface inherited by Cori Metrics providers
 */
class DADI_EXPORT ICori : virtual public IPlugin {
public:
  /**
   * @brief list metrics provided
   * @return Attributes listing provided metrics
   */
  Attributes
  listMetrics() {
    return do_listMetrics();
  }

  /**
   * @brief get metrics requested
   * @param filter Cori request
   * @return Attributes containing metrics requested
   */
  Attributes
  getMetrics(const std::string& filter) {
    return do_getMetrics(filter);
  }

protected:
  /**
   * @brief must be implemented by Cori providers
   * @return
   */
  virtual Attributes
  do_listMetrics() = 0;

  /**
   * @brief must be implemented by Cori providers
   * @param filter
   * @return
   */
  virtual Attributes
  do_getMetrics(const std::string& filter) = 0;
};

} /* namespace dadi */

#endif /* _ICORI_HH_ */

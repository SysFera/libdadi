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

#include "dadi/IPlugin.hh"
#include <string>
#include "dadi/Attributes.hh"

namespace dadi {

class DADI_EXPORT ICori : virtual public IPlugin {
public:
  Attributes
  listMetrics() {
    return do_listMetrics();
  }

  Attributes
  getMetrics(const std::string& filter) {
    return do_getMetrics(filter);
  }

protected:
  virtual Attributes
  do_listMetrics() = 0;

  virtual Attributes
  do_getMetrics(const std::string& filter) = 0;
};

} /* namespace dadi */

#endif /* _ICORI_HH_ */

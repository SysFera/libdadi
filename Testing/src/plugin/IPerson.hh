#ifndef _IPERSON_HH_
#define _IPERSON_HH_

#include <boost/shared_ptr.hpp>

/**
 * @file   Testing/src/plugin/IPerson.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>                   x
 * @brief  IPerson plugin interface
 * @section Licence
 *   |LICENCE|
 *
 */
#include <dadi/IPlugin.hh>

class DADI_EXPORT IPerson : public IPlugin {
public:
  void init() { do_init(); }
  void say_hello() { do_say_hello(); }
protected:
  virtual void do_init()=0;
  virtual void do_say_hello()=0;
};
typedef boost::shared_ptr<IPerson> IPerson_ptr ;
#endif /* _IPERSON_HH_ */

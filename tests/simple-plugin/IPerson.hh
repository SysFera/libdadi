// -*- mode: c++ -*-
#ifndef _IPERSON_HH_
#define _IPERSON_HH_

#include "IPlugin.hh"

class DADI_EXPORT IPerson : public IPlugin {
public:
  void init() { do_init(); }
  void say_hello() { do_say_hello(); }

protected:
  virtual void do_init()=0;
  virtual void do_say_hello()=0;
};


#endif /* _IPERSON_HH_ */

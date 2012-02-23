/**
 * @file   Testing/src/plugin/PersonB.cc
 * @author Ibrahima Cisse <ibrahima.cisse@sysfera.com>
 * @brief  IPerson plugin implementation
 * @section License
 *   |LICENSE|
 *
 */
#include <cstring>
#include <iostream>
#include "IPerson.hh"

class PersonB : public IPerson {
protected:
  virtual void
  do_init() {
    std::cout << "initialization of PersonB\n";
  }
  virtual void
  do_say_hello();
};

void
PersonB::do_say_hello() {
  std::cout << "Hello I'm PersonB!\n";
}


int
create_plugin_instance(void **instance) {
  try {
    *instance = new PersonB;
  } catch (const std::bad_alloc& e) {
    return 1;
  }

  return PLUGIN_OK;
}



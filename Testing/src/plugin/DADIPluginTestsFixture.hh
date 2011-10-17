/**
 * @file   Testing/src/plugin/DADIPluginTestsFixture.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>                   x
 * @brief  libdadi plugin test suite
 * @section Licence
 *   |LICENCE|
 *
 */

// Standard Headers
#include <map>
#include <string>
#include <iostream>
// Boost Headers
#include <boost/foreach.hpp>
// libdadi headers
// Plugins headers
#include "IPerson.hh"

typedef std::map<std::string,IPerson_ptr>::value_type PersonPair;

class AdressBook {

  public:

    void add_person (const std::string& id, const IPerson_ptr& person_ptr ) {

      book[id] = person_ptr ;

    }

    void say_hello() {

      BOOST_FOREACH(PersonPair personPair, book){
        std::cout << personPair.first << ": " ;
        personPair.second->say_hello(); 
      }

    }

  private:
    std::map<std::string,IPerson_ptr> book;

};

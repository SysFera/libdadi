// Standard Headers
#include <cstdlib>
#include <iostream>

// Boost Headers
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>

// libdadi headers
#include "Loader.hh"
#include "Registry.hh"

//Plugins headers
#include "IPerson.hh"


// Test config headers
#include "testconfig.h"




BOOST_AUTO_TEST_SUITE(PluginTests)


  BOOST_AUTO_TEST_CASE(plugin_normal_load) {

    BOOST_TEST_MESSAGE( "[LOADER]: start");

    dadi::Registry reg;
   // Add a new path where the manifest can be loaded
    reg.addPath(MANIFESTOUTPUTPATH);
   
    // load the plugin described into the manifest
    reg.load();
   //  build the expected object 
    IPerson *p = reg.getByInterface<IPerson>("IPerson");

    // is it builded?
    
    BOOST_CHECK (p);
   
    p->init();
    p->say_hello();

    BOOST_TEST_MESSAGE( "[LOADER]: stop ");
  }

// Test bad plugin directory path 

BOOST_AUTO_TEST_CASE(bad_plugin_dir_path) {

  BOOST_TEST_MESSAGE( "[LOADER]: start");

  dadi::Registry reg;
  reg.addPath("bad plugin directory path");

  reg.load();

  BOOST_TEST_MESSAGE( "[LOADER]: stop ");
}


// inexistant plugin interface

BOOST_AUTO_TEST_CASE(bad_plugin_interface) {

  BOOST_TEST_MESSAGE( "[LOADER]: start");

  dadi::Registry reg;
  reg.addPath(MANIFESTOUTPUTPATH);

  reg.load();

  BOOST_CHECK_THROW(reg.getByInterface<IPerson>("bad Interface"),boost::exception);

  BOOST_TEST_MESSAGE( "[LOADER]: stop ");
}


BOOST_AUTO_TEST_SUITE_END()

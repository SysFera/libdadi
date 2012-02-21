/**
 * @file   Testing/src/plugin/DADIPluginTests.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>                   x
 * @brief  libdadi plugin test suite
 * @section Licence
 *   |LICENCE|
 *
 */

// Standard Headers
#include <cstdlib>
#include <iostream>
// Boost Headers
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>
// libdadi headers
#include "dadi/Loader.hh"
#include "dadi/Registry.hh"
#include "dadi/PluginInfo.hh"
// Plugins headers
#include "IPerson.hh"
// Test config headers
#include "testconfig.h"
#include "DADIPluginTestsFixture.hh"

BOOST_AUTO_TEST_SUITE(PluginTests)

// Test  plugin info class bool operator
BOOST_AUTO_TEST_CASE(plugin_bool_operator) {
  BOOST_TEST_MESSAGE("Plugin info bool operator[BEGIN]");
  dadi::PluginInfo pluginInfo;
  BOOST_CHECK(!pluginInfo);
  pluginInfo.name = "some plugin";
  pluginInfo.interface = "some interface";
  pluginInfo.path = "some path";
  pluginInfo.factory = const_cast<char*>("some factory");
  BOOST_CHECK(static_cast<bool>(pluginInfo));
  BOOST_TEST_MESSAGE("Plugin info bool operator[END]");
}

// Test  plugin info class copy constructor
BOOST_AUTO_TEST_CASE(plugin_assignment_operator) {
  BOOST_TEST_MESSAGE("Plugin info copy constructor [BEGIN]");

  dadi::PluginInfo firstPluginInfo;
  firstPluginInfo.name = "some plugin";
  firstPluginInfo.interface = "some interface";
  firstPluginInfo.path = "some path";
  firstPluginInfo.factory = const_cast<char*>("some factory");

  dadi::PluginInfo secondPluginInfo(firstPluginInfo);

  BOOST_CHECK(static_cast<bool>(secondPluginInfo));

  BOOST_CHECK((firstPluginInfo.name == secondPluginInfo.name) &&
              (firstPluginInfo.interface == secondPluginInfo.interface) &&
              (firstPluginInfo.path == secondPluginInfo.path) &&
              (firstPluginInfo.factory == secondPluginInfo.factory));

  BOOST_TEST_MESSAGE("Plugin info copy constructor [END]");
}


// Test Plugin
BOOST_AUTO_TEST_CASE(plugin_normal_load) {
  BOOST_TEST_MESSAGE("[LOADER]: start");
  dadi::Registry& reg= dadi::Registry::instance();
  // Add a new path where the manifest can be loaded
  reg.addPath(MANIFESTOUTPUTPATH);
  // load the plugin described into the manifest
  reg.load();
  // build the expected object
  IPerson *p = reg.getByInterface<IPerson>("IPerson");
  // is it builded?
  BOOST_CHECK(p);
  p->init();
  p->say_hello();
  BOOST_TEST_MESSAGE("[LOADER]: stop ");
}

// Test bad plugin directory path
BOOST_AUTO_TEST_CASE(bad_plugin_dir_path) {
  BOOST_TEST_MESSAGE("[LOADER]: start");
  dadi::Registry& reg= dadi::Registry::instance();
  reg.addPath("bad plugin directory path");
  reg.load();
  BOOST_TEST_MESSAGE("[LOADER]: stop ");
}


// inexistant plugin interface
BOOST_AUTO_TEST_CASE(bad_plugin_interface) {
  BOOST_TEST_MESSAGE("[LOADER]: start");
  dadi::Registry& reg= dadi::Registry::instance();
  reg.addPath(MANIFESTOUTPUTPATH);
  reg.load();
  BOOST_CHECK_THROW(reg.getByInterface<IPerson>("bad Interface"),
                    boost::exception);
  BOOST_TEST_MESSAGE("[LOADER]: stop ");
}

//  Shared Library handling class tests

BOOST_AUTO_TEST_CASE(plugin_class_test) {
  BOOST_TEST_MESSAGE("[LOADER]: start");

  AdressBook addressBook;

  dadi::Registry& reg= dadi::Registry::instance();
  // Add a new path where the manifest can be loaded
  reg.addPath(MANIFESTOUTPUTPATH);
  // load the plugin described into the manifest
  reg.load();
  // build two persons
  IPerson_ptr pAdam(reg.getByName<IPerson>("PersonA"));
  IPerson_ptr pEve(reg.getByName<IPerson>("PersonB"));
  // is it builded?
  BOOST_CHECK(pAdam);
  BOOST_CHECK(pEve);

  addressBook.add_person("Adam", pAdam);
  addressBook.add_person("Eve", pEve);
  // everybody say hello
  addressBook.say_hello();
  BOOST_TEST_MESSAGE("[LOADER]: stop ");
}
BOOST_AUTO_TEST_SUITE_END()

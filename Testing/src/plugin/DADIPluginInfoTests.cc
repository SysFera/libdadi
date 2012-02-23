/**
 * @file   Testing/src/plugin/DADIPluginInfoTests.cc
 * @author Benjamin Depardon <benjamin.depardon@sysfera.com>                   x
 * @brief  libdadi PluginInfo test suite
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
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
// libdadi headers
#include "dadi/Loader.hh"
#include "dadi/Registry.hh"
#include "dadi/PluginInfo.hh"
// Plugins headers
#include "IPerson.hh"
// Test config headers
#include "testconfig.h"
#include "DADIPluginTestsFixture.hh"

BOOST_AUTO_TEST_SUITE(PluginInfoTests)

// Test  plugin info class bool operator
BOOST_AUTO_TEST_CASE(plugin_bool_operator) {
  BOOST_TEST_MESSAGE("Plugin info bool operator[BEGIN]");
  dadi::PluginInfo pluginInfo;
  BOOST_CHECK(!pluginInfo);
  pluginInfo.name = "some plugin";
  BOOST_CHECK(!static_cast<bool>(pluginInfo));
  pluginInfo.interface = "some interface";
  BOOST_CHECK(!static_cast<bool>(pluginInfo));
  pluginInfo.path = "some path";
  BOOST_CHECK(!static_cast<bool>(pluginInfo));
  pluginInfo.factory = const_cast<char*>("some factory");
  BOOST_CHECK(static_cast<bool>(pluginInfo));
  BOOST_TEST_MESSAGE("Plugin info bool operator[END]");
}

// Test  plugin info class copy constructor
BOOST_AUTO_TEST_CASE(plugin_copy_constructor) {
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

// Test  plugin info class assignment operator
BOOST_AUTO_TEST_CASE(plugin_assignment_operator) {
  BOOST_TEST_MESSAGE("Plugin info assignment operator [BEGIN]");

  dadi::PluginInfo firstPluginInfo;
  firstPluginInfo.name = "some plugin";
  firstPluginInfo.interface = "some interface";
  firstPluginInfo.path = "some path";
  firstPluginInfo.factory = const_cast<char*>("some factory");

  dadi::PluginInfo secondPluginInfo = firstPluginInfo;

  BOOST_CHECK(static_cast<bool>(secondPluginInfo));

  BOOST_CHECK((firstPluginInfo.name == secondPluginInfo.name) &&
              (firstPluginInfo.interface == secondPluginInfo.interface) &&
              (firstPluginInfo.path == secondPluginInfo.path) &&
              (firstPluginInfo.factory == secondPluginInfo.factory));

  BOOST_TEST_MESSAGE("Plugin info assignment operator [END]");
}

// Test  plugin info class comparison operator
BOOST_AUTO_TEST_CASE(plugin_comparison_operator) {
  BOOST_TEST_MESSAGE("Plugin info comparison operator [BEGIN]");

  boost::uuids::random_generator gen;
  dadi::PluginInfo firstPluginInfo;
  firstPluginInfo.uuid = gen();
  firstPluginInfo.name = "some plugin";
  firstPluginInfo.interface = "some interface";
  firstPluginInfo.path = "some path";
  firstPluginInfo.factory = const_cast<char*>("some factory");

  dadi::PluginInfo secondPluginInfo = firstPluginInfo;

  BOOST_CHECK(static_cast<bool>(secondPluginInfo));
  // Check equality
  BOOST_CHECK(secondPluginInfo == firstPluginInfo);

  // check difference
  secondPluginInfo.uuid = gen();
  BOOST_CHECK(!(secondPluginInfo == firstPluginInfo));

  BOOST_TEST_MESSAGE("Plugin info comparison operator [END]");
}

// Test  plugin info class different than comparison operator
BOOST_AUTO_TEST_CASE(plugin_different_than_comparison_operator) {
  BOOST_TEST_MESSAGE("Plugin info different than comparison operator [BEGIN]");

  boost::uuids::random_generator gen;
  dadi::PluginInfo firstPluginInfo;
  firstPluginInfo.uuid = gen();
  firstPluginInfo.name = "some plugin";
  firstPluginInfo.interface = "some interface";
  firstPluginInfo.path = "some path";
  firstPluginInfo.factory = const_cast<char*>("some factory");

  dadi::PluginInfo secondPluginInfo = firstPluginInfo;
  secondPluginInfo.uuid = gen();

  BOOST_CHECK(static_cast<bool>(secondPluginInfo));
  BOOST_CHECK(secondPluginInfo != firstPluginInfo);

  BOOST_TEST_MESSAGE("Plugin info different than comparison operator [END]");
}


BOOST_AUTO_TEST_SUITE_END()

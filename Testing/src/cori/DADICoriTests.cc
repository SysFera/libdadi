/**
 * @file DADICoriTests.cc
 * @brief This file implements the libdadi tests for the attributes
 * @author Benjamin Depardon <benjamin.depardon@sysfera.com>
 * @section Licence
 *  |LICENCE|
 *
 */

// C++ Headers
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>
#include "dadi/CoriMgr.hh"
#include "dadi/Config.hh"
#include "dadi/Options.hh"
#include "dadi/Attributes.hh"
#include "dadi/Exception/Attributes.hh"
#include "testconfig.h"

namespace bfs = boost::filesystem;

BOOST_AUTO_TEST_SUITE(CoriTests)

BOOST_AUTO_TEST_CASE(Cori_empty_manager) {
  BOOST_TEST_MESSAGE("# Cori empty manager");

  dadi::Registry& reg = dadi::Registry::instance();
  reg.load();
  dadi::CoriMgr mgr;
  BOOST_REQUIRE_EQUAL(mgr.listPlugins().size(), 0);
  dadi::Attributes emptyAttr;
  dadi::Attributes attr1 = mgr.listMetrics();
  BOOST_REQUIRE(emptyAttr == attr1);
  dadi::Attributes attr2 = mgr.getMetrics("<diet><cori><metrics>"
                                          "<metric>uptime</metric>"
                                          "<metric>ram.total</metric>"
                                          "<metric>swap.total</metric>"
                                          "</metrics></cori></diet>");
  BOOST_REQUIRE(emptyAttr == attr2);
}

BOOST_AUTO_TEST_CASE(Cori_Sigar) {
  BOOST_TEST_MESSAGE("# Cori Sigar");
  dadi::Registry& reg = dadi::Registry::instance();
  boost::filesystem::path manifestDir(MANIFESTOUTPUTPATH);
  reg.addPath(manifestDir.native().c_str());
  reg.load();
  dadi::CoriMgr mgr;
  std::list<std::string> l = mgr.listPlugins();
  BOOST_REQUIRE(mgr.listPlugins().size() != 0);
  dadi::Attributes emptyAttr;
  dadi::Attributes attr1 = mgr.listMetrics();

  BOOST_REQUIRE(emptyAttr != attr1);
  dadi::Attributes attr2 = mgr.getMetrics("<diet><cori><metrics>"
                                          "<metric>uptime</metric>"
                                          "<metric>ram.total</metric>"
                                          "<metric>swap.total</metric>"
                                          "</metrics></cori></diet>");
  BOOST_REQUIRE(emptyAttr != attr2);
}


BOOST_AUTO_TEST_SUITE_END()

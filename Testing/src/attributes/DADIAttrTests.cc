/**
 * @file DADIAttrTests.cc
 * @brief This file implements the libdadi tests for the attributes
 * @author Kevin COULOMB (kevin.coulomb@sysfera.com)
 */


// C++ Headers
#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include "Config.hh"
#include "Options.hh"
#include "Attributes.hh"


using namespace std;
using namespace dadi;

BOOST_AUTO_TEST_SUITE(Dadi_test_attr)

BOOST_AUTO_TEST_CASE(getAttr_ok) {
  BOOST_TEST_MESSAGE("# Get attribute normal call");
  Attributes attr;
  string s;
  attr.putAttr<string>("/tmp", "toto");
  s = attr.getAttr<string>("/tmp", "");
  BOOST_REQUIRE("toto" == s);
}

BOOST_AUTO_TEST_CASE(getAttrNoParam_ok) {
  BOOST_TEST_MESSAGE("# Get attribute normal call no value");
  Attributes attr;
  string s;
  s = attr.getAttr<string>("/tmp", "toto");
  BOOST_REQUIRE("toto" == s);
}

// TODO: Pertinence de ce test ?
BOOST_AUTO_TEST_CASE(putAttr_ok) {
  BOOST_TEST_MESSAGE("# Get attribute normal call");
  Attributes attr;
  attr.putAttr<std::string>("/tmp", "toto");
}

BOOST_AUTO_TEST_SUITE_END()

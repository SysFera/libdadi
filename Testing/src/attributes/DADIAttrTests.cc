/**
 * @file DADIAttrTests.cc
 * @brief This file implements the libdadi tests for the attributes
 * @author Kevin COULOMB (kevin.coulomb@sysfera.com)
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
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
#include "dadi/Config.hh"
#include "dadi/Options.hh"
#include "dadi/Attributes.hh"
#include "dadi/Exception/Attributes.hh"
#include "testconfig.h"

namespace bfs = boost::filesystem;

BOOST_AUTO_TEST_SUITE(AttributesTests)

BOOST_AUTO_TEST_CASE(getAttr_ok) {
  BOOST_TEST_MESSAGE("# Get attribute normal call");
  dadi::Attributes attr;
  std::string s;
  attr.putAttr<std::string>("/tmp", "toto");
  s = attr.getAttr<std::string>("/tmp", "");
  BOOST_REQUIRE_EQUAL("toto", s);
}

BOOST_AUTO_TEST_CASE(getAttrNoParam_ok) {
  BOOST_TEST_MESSAGE("# Get attribute normal call no value");
  dadi::Attributes attr;
  std::string s;
  s = attr.getAttr<std::string>("/tmp", "toto");
  BOOST_REQUIRE_EQUAL("toto", s);
}

// TODO: Pertinence de ce test ?
BOOST_AUTO_TEST_CASE(putAttr_ok) {
  BOOST_TEST_MESSAGE("# Get attribute normal call");
  dadi::Attributes attr;
  attr.putAttr<std::string>("/tmp", "toto");
}

BOOST_AUTO_TEST_CASE(getAttr_exception_unknown_attr) {
  BOOST_TEST_MESSAGE("# Get Unknown attribute");
  dadi::Attributes attr;
  BOOST_REQUIRE_THROW(attr.getAttr<std::string>("toto"),
                      dadi::UnknownAttributeError);
}

BOOST_AUTO_TEST_CASE(getAttr_exception_invalid_attr_1) {
  BOOST_TEST_MESSAGE("# Get invalid attribute");
  dadi::Attributes attr;
  attr.putAttr<std::string>("toto", "toto");
  // "toto" >> i (int) KO
  BOOST_REQUIRE_THROW(attr.getAttr<int>("toto"),
                      dadi::InvalidAttributeError);
}

BOOST_AUTO_TEST_CASE(getAttr_exception_invalid_attr_nothrow_1) {
  BOOST_TEST_MESSAGE("# Get invalid attribute");
  dadi::Attributes attr;
  attr.putAttr<std::string>("toto", "1");
  // "1" >> i (int) OK
  BOOST_REQUIRE_NO_THROW(attr.getAttr<int>("toto"));
}

BOOST_AUTO_TEST_CASE(getAttr_exception_invalid_attr_nothrow_2) {
  BOOST_TEST_MESSAGE("# Get invalid attribute");
  dadi::Attributes attr;
  attr.putAttr<int>("toto", 1);
  // 1 >> s (string) OK
  BOOST_REQUIRE_NO_THROW(attr.getAttr<std::string>("toto"));
}

BOOST_AUTO_TEST_CASE(attr_copy_ctor) {
  BOOST_TEST_MESSAGE("# Attributes copy ctor");
  dadi::Attributes attr1;
  attr1.putAttr<std::string>("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  dadi::Attributes attr2(attr1);

  BOOST_REQUIRE(attr1 == attr2);
}

BOOST_AUTO_TEST_CASE(attr_copy_operator) {
  BOOST_TEST_MESSAGE("# Attributes copy operator");
  dadi::Attributes attr1;
  attr1.putAttr<std::string>("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  dadi::Attributes attr2 = attr1;

  BOOST_REQUIRE(attr1 == attr2);
}

BOOST_AUTO_TEST_CASE(attr_equal_operator) {
  BOOST_TEST_MESSAGE("# Attributes equal operator");
  dadi::Attributes attr1;
  attr1.putAttr<std::string>("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);

  dadi::Attributes attr2;
  attr2.putAttr<std::string>("string", "toto");
  attr2.putAttr("int", 1);
  attr2.putAttr("float", 1.2);

  BOOST_REQUIRE(attr1 == attr2);
}

BOOST_AUTO_TEST_CASE(attr_different_operator) {
  BOOST_TEST_MESSAGE("# Attributes different operator");
  dadi::Attributes attr1;
  attr1.putAttr<std::string>("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);

  dadi::Attributes attr2;
  attr2.putAttr<std::string>("string", "toto");
  attr2.putAttr("int", 2);
  attr2.putAttr("float", 1.2);

  BOOST_REQUIRE(attr1 != attr2);
}

BOOST_AUTO_TEST_CASE(attr_load_attr_file_xml) {
  BOOST_TEST_MESSAGE("# Attributes loadAttr from file");
  // load serialized data from file
  bfs::path p(TESTFILESOUTPUTPATH);
  p /= "attr_sample01.xml";
  bfs::ifstream ifs(p);
  std::stringstream ss;
  ss << ifs.rdbuf();
  dadi::Attributes attr1;
  attr1.loadAttr(ss.str());
  // check that data is correctly loaded
  std::string s = attr1.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr1.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr1.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);
}

BOOST_AUTO_TEST_CASE(attr_load_attr_file_ini) {
  BOOST_TEST_MESSAGE("# Attributes loadAttr from ini file");
  // load serialized data from file
  bfs::path p(TESTFILESOUTPUTPATH);
  p /= "attr_sample01.ini";
  bfs::ifstream ifs(p);
  std::stringstream ss;
  ss << ifs.rdbuf();
  dadi::Attributes attr1;
  BOOST_TEST_MESSAGE("## "+ ss.str());

  attr1.loadAttr(ss.str(), dadi::FORMAT_INI);
  // check that data is correctly loaded
  std::string s = attr1.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr1.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr1.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);
}

BOOST_AUTO_TEST_CASE(attr_load_attr_file_json) {
  BOOST_TEST_MESSAGE("# Attributes loadAttr from json file");
  // load serialized data from file
  bfs::path p(TESTFILESOUTPUTPATH);
  p /= "attr_sample01.json";
  bfs::ifstream ifs(p);
  std::stringstream ss;
  ss << ifs.rdbuf();
  dadi::Attributes attr1;
  attr1.loadAttr(ss.str(), dadi::FORMAT_JSON);
  // check that data is correctly loaded
  std::string s = attr1.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr1.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr1.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);
}

/* getAttrList tests */
BOOST_AUTO_TEST_CASE(getAttrList_exception_unknown_attr) {
  BOOST_TEST_MESSAGE("# Get Unknown attribute list");
  dadi::Attributes attr;
  BOOST_REQUIRE_THROW(attr.getAttrList<std::vector<std::string> >("toto"),
                      dadi::UnknownAttributeError);
}

// BOOST_AUTO_TEST_CASE(getAttrList_exception_invalid_attr_1) {
//   BOOST_TEST_MESSAGE("# Get invalid attribute List");
//   dadi::Attributes attr;
//   attr.putAttr<std::string>("toto", 1);
//   // "toto" >> i (vector<string>) KO
//   BOOST_REQUIRE_THROW(attr.getAttrList<std::vector<int> >("toto"),
//                       dadi::InvalidAttributeError);
// }

BOOST_AUTO_TEST_CASE(getAttrList_valid_attr) {
  BOOST_TEST_MESSAGE("# Get valid attribute list");
  dadi::Attributes attr;
  attr.putAttr<std::string>("toto", "toto");
  // "toto" >> i (vector<string>) OK
  BOOST_REQUIRE_NO_THROW(attr.getAttrList<std::vector<std::string> >("toto"));
}

BOOST_AUTO_TEST_CASE(getAttrList_valid_attr2) {
  BOOST_TEST_MESSAGE("# Get valid attribute list with 2 elements");
  dadi::Attributes attr;
  attr.putAttr<std::string>("toto", "toto1");
  attr.putAttr<std::string>("toto", "toto2");
  // "toto" >> i (vector<string>) OK
  std::vector<std::string> listAttr;
  BOOST_REQUIRE_NO_THROW(listAttr = attr.getAttrList<std::vector<std::string> >("toto"));
  std::cout << listAttr.size() << std::endl;

  listAttr = attr.getAttrList<std::vector<std::string> >("toto");

  BOOST_FOREACH(const std::string& t, listAttr) {
    std::cout << t << std::endl;

  }


  BOOST_CHECK(listAttr.size() == 2);
}

// BOOST_AUTO_TEST_CASE(getAttr_exception_invalid_attr_nothrow_1) {
//   BOOST_TEST_MESSAGE("# Get invalid attribute");
//   dadi::Attributes attr;
//   attr.putAttr<std::string>("toto", "1");
//   // "1" >> i (int) OK
//   BOOST_REQUIRE_NO_THROW(attr.getAttr<int>("toto"));
// }

// BOOST_AUTO_TEST_CASE(getAttr_exception_invalid_attr_nothrow_2) {
//   BOOST_TEST_MESSAGE("# Get invalid attribute");
//   dadi::Attributes attr;
//   attr.putAttr<int>("toto", 1);
//   // 1 >> s (string) OK
//   BOOST_REQUIRE_NO_THROW(attr.getAttr<std::string>("toto"));
// }


BOOST_AUTO_TEST_SUITE_END()

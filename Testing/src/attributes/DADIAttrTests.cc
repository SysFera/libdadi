/**
 * @file DADIAttrTests.cc
 * @brief This file implements the libdadi tests for the attributes
 * @author Kevin COULOMB (kevin.coulomb@sysfera.com)
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
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

BOOST_AUTO_TEST_CASE(attr_equal_comparison_operator) {
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

BOOST_AUTO_TEST_CASE(attr_load_attr_file_info) {
  BOOST_TEST_MESSAGE("# Attributes loadAttr from info file");
  // load serialized data from file
  bfs::path p(TESTFILESOUTPUTPATH);
  p /= "attr_sample01.nfo";
  bfs::ifstream ifs(p);
  std::stringstream ss;
  ss << ifs.rdbuf();
  dadi::Attributes attr1;
  attr1.loadAttr(ss.str(), dadi::FORMAT_INFO);
  // check that data is correctly loaded
  std::string s = attr1.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr1.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr1.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);
}


BOOST_AUTO_TEST_CASE(attr_save_attr_format_info) {
  BOOST_TEST_MESSAGE("# Attributes saveAttr into an info format");
  std::stringstream ss;

  // save the attribute into a stream
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  ss << attr1.saveAttr(dadi::FORMAT_INFO);

  // Now load what has been saved
  dadi::Attributes attr2;
  attr2.loadAttr(ss.str(), dadi::FORMAT_INFO);
  // check that data is correctly loaded
  std::string s = attr2.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr2.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr2.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);

  BOOST_REQUIRE(attr1 == attr2);
}

BOOST_AUTO_TEST_CASE(attr_save_attr_format_xml) {
  BOOST_TEST_MESSAGE("# Attributes saveAttr into an xml format");
  std::stringstream ss;

  // save the attribute into a stream
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  ss << attr1.saveAttr(dadi::FORMAT_XML);

  // Now load what has been saved
  dadi::Attributes attr2;
  attr2.loadAttr(ss.str(), dadi::FORMAT_XML);
  // check that data is correctly loaded
  std::string s = attr2.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr2.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr2.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);

  BOOST_REQUIRE(attr1 == attr2);
}

BOOST_AUTO_TEST_CASE(attr_save_attr_format_json) {
  BOOST_TEST_MESSAGE("# Attributes saveAttr into a json format");
  std::stringstream ss;

  // save the attribute into a stream
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  ss << attr1.saveAttr(dadi::FORMAT_JSON);

  // Now load what has been saved
  dadi::Attributes attr2;
  attr2.loadAttr(ss.str(), dadi::FORMAT_JSON);
  // check that data is correctly loaded
  std::string s = attr2.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr2.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr2.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);

  BOOST_REQUIRE(attr1 == attr2);
}


BOOST_AUTO_TEST_CASE(attr_save_attr_format_ini) {
  BOOST_TEST_MESSAGE("# Attributes saveAttr into an ini format");
  std::stringstream ss;

  // save the attribute into a stream
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  ss << attr1.saveAttr(dadi::FORMAT_INI);

  // Now load what has been saved
  dadi::Attributes attr2;
  attr2.loadAttr(ss.str(), dadi::FORMAT_INI);
  // check that data is correctly loaded
  std::string s = attr2.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr2.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr2.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);

  BOOST_REQUIRE(attr1 == attr2);
}


BOOST_AUTO_TEST_CASE(attr_constructor_format_ini) {
  BOOST_TEST_MESSAGE(
    "# Attributes constructor based on a string in an ini format");
  std::stringstream ss;

  // save the attribute into a stream
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  ss << attr1.saveAttr(dadi::FORMAT_INI);

  // Now load what has been saved
  dadi::Attributes attr2(ss.str(), dadi::FORMAT_INI);
  // check that data is correctly loaded
  std::string s = attr2.getAttr<std::string>("string");
  BOOST_REQUIRE_EQUAL("toto", s);
  int i = attr2.getAttr<int>("int");
  BOOST_REQUIRE_EQUAL(1, i);
  float f = attr2.getAttr<float>("float");
  // coz' this is floating numbers, bro !
  BOOST_REQUIRE_CLOSE(1.2, f, 0.0001);

  BOOST_REQUIRE(attr1 == attr2);
}


BOOST_AUTO_TEST_CASE(attr_load_attr_bad_format) {
  BOOST_TEST_MESSAGE("# Attributes load bad format");
  std::stringstream ss;
  ss << "bad XML format";

  // Now load what has been "saved"
  dadi::Attributes attr2;
  BOOST_CHECK_THROW(attr2.loadAttr(ss.str(), dadi::FORMAT_XML),
                    dadi::ParsingAttributeError);
}


BOOST_AUTO_TEST_CASE(attr_equal_operator) {
  BOOST_TEST_MESSAGE("# Attributes equal operator");
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);

  dadi::Attributes attr2;
  attr2 = attr1;

  BOOST_REQUIRE(attr1 == attr2);
}

BOOST_AUTO_TEST_CASE(attr_merge) {
  BOOST_TEST_MESSAGE("# Attributes merge");
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  attr1.addAttr("holy", "god");
  attr1.addAttr("holy.character", "Patsy");
  attr1.addAttr("holy.character", "Robin");
  attr1.addAttr("holy.weapon", "sword");

  dadi::Attributes attr2;
  attr2.putAttr("float", 1.2);
  attr2.putAttr("int2", 2);
  attr2.putAttr("holy.grail", "lost");
  attr2.addAttr("holy.character", "Arthur");
  attr2.addAttr("holy.character", "Lancelot");
  attr2.addAttr("holy.character", "Patsy");
  attr2.addAttr("holy.castle", "arggghhh");

  attr1.merge(attr2);
  BOOST_CHECK_NO_THROW(attr1.getAttr<std::string>("holy"));
  BOOST_REQUIRE_EQUAL(attr1.getAttr<int>("int2"), 2);
  BOOST_REQUIRE_EQUAL(attr1.getAttr<std::string>("holy"), "god");
  BOOST_REQUIRE_EQUAL(attr1.getAttr<std::string>("holy.castle"), "arggghhh");
  BOOST_REQUIRE_EQUAL(attr1.getAttr<std::string>("holy.weapon"), "sword");
  BOOST_REQUIRE_EQUAL(attr1.getAttr<std::string>("holy.grail"), "lost");
}



BOOST_AUTO_TEST_CASE(attr_str) {
  BOOST_TEST_MESSAGE("# Attributes str");
  dadi::Attributes attr1;
  attr1.putAttr("string", "toto");
  attr1.putAttr("int", 1);
  attr1.putAttr("float", 1.2);
  std::string xml;
  xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  xml += "<string>toto</string>\n<int>1</int>\n<float>1.2</float>\n";
  BOOST_CHECK_NO_THROW(dadi::str(attr1, dadi::FORMAT_XML));
  BOOST_CHECK_EQUAL(dadi::str(attr1, dadi::FORMAT_XML), xml);
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
//   attr.putAttr<std::string>("toto", "toto");
//   // "toto" >> i (vector<int>) KO
//   BOOST_REQUIRE_THROW(attr.getAttrList<std::vector<int> >("toto"),
//                       dadi::InvalidAttributeError);
// }

// BOOST_AUTO_TEST_CASE(getAttrList_valid_attr) {
//   BOOST_TEST_MESSAGE("# Get valid attribute list");
//   dadi::Attributes attr;
//   attr.putAttr<std::string>("toto", "toto");
//   // "toto" >> i (vector<string>) OK
//   BOOST_REQUIRE_NO_THROW(attr.getAttrList<std::vector<std::string> >("toto"));
// }


BOOST_AUTO_TEST_CASE(getAttrList_valid_attr_1) {
  BOOST_TEST_MESSAGE("# Get valid int attribute list");
  dadi::Attributes attr;
  attr.putAttr<std::string>("toto.uu", "1");
  // "toto" >> i (vector<int>) OK
  BOOST_REQUIRE_NO_THROW(attr.getAttrList<std::vector<int> >("toto.uu"));
  BOOST_CHECK(attr.getAttrList<std::vector<int> >("toto.uu")[0] == 1);
}

BOOST_AUTO_TEST_CASE(getAttrList_valid_attr_2) {
  BOOST_TEST_MESSAGE("# Get valid int attribute list 2");
  dadi::Attributes attr;
  attr.putAttr<int>("toto.sldfk", 1);
  // "toto" >> i (vector<int>) OK
  BOOST_REQUIRE_NO_THROW(attr.getAttrList<std::vector<int> >("toto.sldfk"));
  BOOST_CHECK(attr.getAttrList<std::vector<int> >("toto.sldfk")[0] == 1);
}

BOOST_AUTO_TEST_CASE(getAttrList_valid_attr2) {
  BOOST_TEST_MESSAGE("# Get valid attribute list with an empty elements");
  dadi::Attributes attr;
  attr.addAttr<std::string>("toto.uu.titi", "toto1");
  attr.addAttr<std::string>("toto.uu.titi", "toto2");
  attr.addAttr<std::string>("toto.uu.tata", "toto3");
  // "toto" >> i (vector<string>) OK
  std::vector<std::string> listAttr;
  BOOST_REQUIRE_NO_THROW(listAttr = attr.getAttrList<std::vector<std::string> >("toto.uu"));
  BOOST_CHECK_EQUAL(listAttr.size(), 1);
  // we retrieve the value at toto.uu, which should be empty
  BOOST_CHECK(listAttr[0].empty());
}

BOOST_AUTO_TEST_CASE(getAttrList_valid_attr2bis) {
  BOOST_TEST_MESSAGE("# Get valid attribute list with 2 elements");
  dadi::Attributes attr;
  attr.addAttr<std::string>("toto.uu.titi", "toto1");
  attr.addAttr<std::string>("toto.uu.titi", "toto2");
  attr.addAttr<std::string>("toto.uu.tata", "toto3");
  // "toto" >> i (vector<string>) OK
  std::vector<std::string> listAttr;
  BOOST_REQUIRE_NO_THROW(listAttr = attr.getAttrList<std::vector<std::string> >("toto.uu.titi"));
  BOOST_CHECK_EQUAL(listAttr.size(), 2);
  BOOST_CHECK(!listAttr[0].empty());
  BOOST_CHECK(!listAttr[1].empty());
}

BOOST_AUTO_TEST_CASE(getAttrList_valid_attr3) {
  BOOST_TEST_MESSAGE("# Get valid attribute list with 2 elements, no subkey");
  dadi::Attributes attr;
  attr.addAttr<std::string>("toto", "toto1");
  attr.addAttr<std::string>("toto", "toto2");
  attr.addAttr<std::string>("toto.uu.tata", "toto3");
  attr.addAttr<std::string>("tata", "toto4");
  // "toto" >> i (vector<string>) OK
  std::vector<std::string> listAttr;
  BOOST_REQUIRE_NO_THROW(listAttr = attr.getAttrList<std::vector<std::string> >("toto"));
  BOOST_CHECK_EQUAL(listAttr.size(), 1);
  BOOST_CHECK(!listAttr[0].empty());
}


BOOST_AUTO_TEST_CASE(getAttrList_exception_invalid_attr_nothrow_1) {
  BOOST_TEST_MESSAGE("# Get invalid attribute");
  dadi::Attributes attr;
  attr.putAttr<std::string>("toto.int", "1");
  attr.putAttr<std::string>("toto.int", "2");
  // "1" >> i (int) OK
  // "2" >> i (int) OK
  BOOST_REQUIRE_NO_THROW(attr.getAttr<int>("toto.int"));
}

BOOST_AUTO_TEST_CASE(getAttrList_exception_invalid_attr_nothrow_2) {
  BOOST_TEST_MESSAGE("# Get invalid attribute");
  dadi::Attributes attr;
  attr.putAttr<int>("toto.int", 1);
  attr.putAttr<int>("toto.int", 2);
  // 1 >> s (string) OK
  // 2 >> s (string) OK
  BOOST_REQUIRE_NO_THROW(attr.getAttr<std::string>("toto"));
}


BOOST_AUTO_TEST_SUITE_END()

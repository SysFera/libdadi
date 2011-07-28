/**
 * @file DADIMessageTests.cc
 * @brief This file implements the libdadi tests for messages
 * @author Benjamin DEPARDON (benjamin.depardon@sysfera.com)
 */

#include <iostream>
#include <Logging/Message.hh>
#include "Config.hh"
#include "Options.hh"

// C++ Headers
#include <iostream>
#include <sstream>
#include <cmath>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dadi;

BOOST_AUTO_TEST_SUITE(Dadi_Message_test)

BOOST_AUTO_TEST_CASE(default_constructor_test) {
  BOOST_TEST_MESSAGE("#Test default constructor#");
  Message myMsg = Message();

  // Check line == 0
  BOOST_REQUIRE_EQUAL(myMsg.getLine(), 0);
  // Check Prio == Message::PRIO_FATAL
  BOOST_REQUIRE_EQUAL(myMsg.getPriority(), Message::PRIO_FATAL);
}

BOOST_AUTO_TEST_CASE(constructor_3params_test) {
  BOOST_TEST_MESSAGE("#Test constructor with 3 parameters#");
  string source = "Bridgekeeper";
  string msgToLog = "What... is the air-speed velocity of an unladen swallow?";
  Message myMsg = Message(source, msgToLog, Message::PRIO_DEBUG);

  // Check source
  BOOST_REQUIRE_EQUAL(myMsg.getSource(), source);
  // Check text
  BOOST_REQUIRE_EQUAL(myMsg.getText(), msgToLog);
  // Check Priority
  BOOST_REQUIRE_EQUAL(myMsg.getPriority(), Message::PRIO_DEBUG);
}


BOOST_AUTO_TEST_CASE(constructor_5params_test) {
  BOOST_TEST_MESSAGE("#Test constructor with 5 parameters#");
  string source = "Bridgekeeper";
  string msgToLog = "What... is the air-speed velocity of an unladen swallow?";
  string file = "Grail.holy";
  int line = 42;
  Message myMsg = Message(source, msgToLog, Message::PRIO_DEBUG, file, line);

  // Check source
  BOOST_REQUIRE_EQUAL(myMsg.getSource(), source);
  // Check text
  BOOST_REQUIRE_EQUAL(myMsg.getText(), msgToLog);
  // Check Priority
  BOOST_REQUIRE_EQUAL(myMsg.getPriority(), Message::PRIO_DEBUG);
  // Check File
  BOOST_REQUIRE_EQUAL(myMsg.getFile(), file);
  // Check Line
  BOOST_REQUIRE_EQUAL(myMsg.getLine(), line);
}


BOOST_AUTO_TEST_CASE(setters_getters_test) {
  BOOST_TEST_MESSAGE("#Test setters and getters#");
  Message myMsg = Message();

  string source = "Bridgekeeper";
  string msgToLog = "What... is the air-speed velocity of an unladen swallow?";
  string file = "Grail.holy";
  int line = 42;
  long timestamp = 1234567;

  // Set source
  myMsg.setSource(source);
  // Check source
  BOOST_REQUIRE_EQUAL(myMsg.getSource(), source);

  // Set text
  myMsg.setText(msgToLog);
  // Check text
  BOOST_REQUIRE_EQUAL(myMsg.getText(), msgToLog);

  // Set priority
  myMsg.setPriority(Message::PRIO_DEBUG);
  // Check Priority
  BOOST_REQUIRE_EQUAL(myMsg.getPriority(), Message::PRIO_DEBUG);

  // Set file
  myMsg.setFile(file);
  // Check File
  BOOST_REQUIRE_EQUAL(myMsg.getFile(), file);

  // Set line
  myMsg.setLine(line);
  // Check Line
  BOOST_REQUIRE_EQUAL(myMsg.getLine(), line);

  // Set timestamp
  myMsg.setTimestamp(timestamp);
  // Check Line
  BOOST_REQUIRE_EQUAL(myMsg.getTimestamp(), timestamp);
}

BOOST_AUTO_TEST_CASE(attributes_test) {
  BOOST_TEST_MESSAGE("#Test message attributes#");
  Message myMsg = Message();

  string key = "Bridgekeeper";
  string value = "What... is the air-speed velocity of an unladen swallow?";

  // add an attribute
  myMsg[key] = value;
  // Check attribute
  BOOST_REQUIRE_EQUAL(myMsg[key], value);
}


BOOST_AUTO_TEST_SUITE_END()

// THE END

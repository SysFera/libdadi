/**
 * @file DADILoggingTests.cc
 * @brief This file implements the libdadi tests for logger
 * @author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 */

#include <iostream>
#include <Logging/ConsoleChannel.hh>
#include <Logging/FileChannel.hh>
#include <Logging/Logger.hh>
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
namespace bfs=boost::filesystem; // an alias for boost filesystem namespace
static const string temporaryFilename = "crap4.log";

BOOST_AUTO_TEST_SUITE (Dadi_test)


BOOST_AUTO_TEST_CASE( initialize_logger_normal_call) {

  BOOST_TEST_MESSAGE("#Initialize logger normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("initialize_logger");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
}

BOOST_AUTO_TEST_CASE( get_logger_name_normal_call) {

  BOOST_TEST_MESSAGE("#Get logger name normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("logger_name");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  BOOST_REQUIRE(mylogger1->getName().compare("logger_name") == 0);
}

BOOST_AUTO_TEST_CASE( get_level_normal_call) {

  BOOST_TEST_MESSAGE("#Get level normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("get_level_normal");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_TRACE);
  //To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_TRACE);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  //To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_DEBUG);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_INFORMATION);
  //To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_INFORMATION);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_WARNING);
  //To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_WARNING);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_ERROR);
  //To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_ERROR);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_FATAL);
  //To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_FATAL);
}

BOOST_AUTO_TEST_CASE( get_level_bad_call) {

  BOOST_TEST_MESSAGE("#Get level bad call (put nonexistent priority)#");

  LoggerPtr mylogger1 = Logger::getLogger("get_level_bad");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  //To put a nonexistent level
  mylogger1->setLevel(12);
  //To get the priority level
  BOOST_REQUIRE((mylogger1->getLevel() >= 1) && (mylogger1->getLevel() <= 7));
}

BOOST_AUTO_TEST_CASE( get_channel_normal_call) {

  BOOST_TEST_MESSAGE("#Get channel normal call#");
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("get_channel_normal");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  ChannelPtr cc1(new ConsoleChannel(oss));
  BOOST_REQUIRE(cc1);
  //To set a channel
  mylogger1->setChannel(cc1);
  //To get the channel
  ChannelPtr cc2 = mylogger1->getChannel();
  BOOST_REQUIRE(cc2);
}

BOOST_AUTO_TEST_CASE( get_channel_bad_call) {

  BOOST_TEST_MESSAGE("#Get channel bad#");
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("get_channel_bad");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  //To get the channel
  ChannelPtr cc1 = mylogger1->getChannel();
  BOOST_REQUIRE(!cc1);
}

BOOST_AUTO_TEST_CASE( log_on_file_normal_call) {

  BOOST_TEST_MESSAGE("#FileChannel test normal call#");
  //The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  LoggerPtr mylogger1 = Logger::getLogger("fileChannel_normal");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  ChannelPtr cc1(new FileChannel);
  cc1->putAttr<std::string>("path", temporaryFilename);
  mylogger1->setChannel(cc1);
  //To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
  //To check that the file created is not empty
  BOOST_REQUIRE(((FileChannel* )cc1.get())->getSize() != 0);
  bfs::path file (temporaryFilename);
  //To check that the file with message logged is created
  BOOST_REQUIRE(bfs::exists(file));
  bfs::fstream ifs (file);
  std::string ss;
  std::getline(ifs, ss);
  //To check that the file contains the message to log
  BOOST_REQUIRE(ss.compare(msgToLog) == 0);
  //To remove the file created
  boost::filesystem::remove(file.filename());
}

BOOST_AUTO_TEST_CASE( log_on_console_normal_call) {

  BOOST_TEST_MESSAGE("#ConsoleChannel test normal call#");
  //The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("ConsoleChannel_normal");
  //To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  //To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  ChannelPtr cc1(new ConsoleChannel(oss));
  mylogger1->setChannel(cc1);
  //To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
  //To check that the console content is the same that the message to log
  BOOST_REQUIRE(oss.str().compare(msgToLog+"\n") == 0);
}

BOOST_AUTO_TEST_SUITE_END()

// THE END

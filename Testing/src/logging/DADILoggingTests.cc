/**
 * @file DADILoggingTests.cc
 * @brief This file implements the libdadi tests for logger
 * @author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * @author Benjamin DEPARDON (benjamin.depardon@sysfera.com)
 */

#include <iostream>
#include <Logging/ConsoleChannel.hh>
#include <Logging/FileChannel.hh>
#include <Logging/NullChannel.hh>
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
namespace bfs = boost::filesystem;  // an alias for boost filesystem namespace
static const string temporaryFilename("crap4.log");

BOOST_AUTO_TEST_SUITE(Dadi_Logging_test)

BOOST_AUTO_TEST_CASE(initialize_logger_normal_call) {

  BOOST_TEST_MESSAGE("#Initialize logger normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("initialize_logger");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
}

BOOST_AUTO_TEST_CASE(get_logger_name_normal_call) {

  BOOST_TEST_MESSAGE("#Get logger name normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("logger_name");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  BOOST_REQUIRE(mylogger1->getName().compare("logger_name") == 0);
}

BOOST_AUTO_TEST_CASE(get_level_normal_call) {

  BOOST_TEST_MESSAGE("#Get level normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("get_level_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_TRACE);
  // To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_TRACE);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  // To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_DEBUG);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_INFORMATION);
  // To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_INFORMATION);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_WARNING);
  // To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_WARNING);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_ERROR);
  // To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_ERROR);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_FATAL);
  // To get the priority level
  BOOST_REQUIRE(mylogger1->getLevel() == Message::PRIO_FATAL);
}

BOOST_AUTO_TEST_CASE(is_normal_call) {

  BOOST_TEST_MESSAGE("#function is normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("is_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  BOOST_REQUIRE(mylogger1->is(Message::PRIO_INFORMATION));
  BOOST_REQUIRE(!mylogger1->is(Message::PRIO_TRACE));
}

BOOST_AUTO_TEST_CASE(is_bad_call) {

  BOOST_TEST_MESSAGE("#function is bad call#");
  LoggerPtr mylogger1 = Logger::getLogger("is_bad");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);

  // put invalid low priority level
  mylogger1->setLevel(Message::PRIO_TRACE-1);
  // Level must be set to Message::PRIO_INFORMATION
  BOOST_REQUIRE(mylogger1->is(Message::PRIO_INFORMATION));

  // put invalid high priority level
  mylogger1->setLevel(Message::PRIO_FATAL+1);
  // Level must be set to Message::PRIO_INFORMATION
  BOOST_REQUIRE(mylogger1->is(Message::PRIO_INFORMATION));
}

BOOST_AUTO_TEST_CASE(check_trace_function_normal_call) {

  BOOST_TEST_MESSAGE("#Check trace function normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("check_trace_function_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level trace
  mylogger1->setLevel(Message::PRIO_TRACE);
  // To check the function
  BOOST_REQUIRE(mylogger1->trace());
  // To put another priority level
  mylogger1->setLevel(Message::PRIO_WARNING);
  // To check the function
  BOOST_REQUIRE(!mylogger1->trace());
}

BOOST_AUTO_TEST_CASE(check_debug_function_normal_call) {

  BOOST_TEST_MESSAGE("#Check debug function normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("check_debug_function_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  // To check the function
  BOOST_REQUIRE(mylogger1->debug());  // should pass PRIO_DEBUG >= PRIO_DEBUG
  // To put the another priority level
  mylogger1->setLevel(Message::PRIO_TRACE);
  // To check the function
  BOOST_REQUIRE(mylogger1->debug());  // should pass PRIO_DEBUG >= PRIO_TRACE
}

BOOST_AUTO_TEST_CASE(check_information_function_normal_call) {

  BOOST_TEST_MESSAGE("#Check information function normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("check_information_function_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_INFORMATION);
  // To check the function
  // should pass PRIO_INFORMATION >= PRIO_INFORMATION
  BOOST_REQUIRE(mylogger1->information());
  // To put another priority level
  mylogger1->setLevel(Message::PRIO_TRACE);
  // To check the function
  // should pass PRIO_INFORMATION >= PRIO_TRACE
  BOOST_REQUIRE(mylogger1->information());
}

BOOST_AUTO_TEST_CASE(check_warning_function_normal_call) {

  BOOST_TEST_MESSAGE("#Check warning function normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("check_warning_function_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_WARNING);
  // To check the function
  // should pass PRIO_WARNING >= PRIO_WARNING
  BOOST_REQUIRE(mylogger1->warning());
  // To put another priority level
  mylogger1->setLevel(Message::PRIO_TRACE);
  // To check the function
  // should pass PRIO_WARNING >= PRIO_TRACE
  BOOST_REQUIRE(mylogger1->warning());
}

BOOST_AUTO_TEST_CASE(check_error_function_normal_call) {

  BOOST_TEST_MESSAGE("#Check error function normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("check_error_function_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_ERROR);
  // To check the function
  // should pass PRIO_ERROR >= PRIO_ERROR
  BOOST_REQUIRE(mylogger1->error());
  // To put another priority level
  mylogger1->setLevel(Message::PRIO_TRACE);
  // To check the function
  // should pass PRIO_ERROR >= PRIO_TRACE
  BOOST_REQUIRE(mylogger1->error());
}

BOOST_AUTO_TEST_CASE(check_fatal_function_normal_call) {

  BOOST_TEST_MESSAGE("#Check fatal function normal call#");
  LoggerPtr mylogger1 = Logger::getLogger("check_fatal_function_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_FATAL);
  // To check the function
  // should pass PRIO_FATAL >= PRIO_FATAL
  BOOST_REQUIRE(mylogger1->fatal());
  // To put another priority level
  mylogger1->setLevel(Message::PRIO_TRACE);
  // To check the function
  // should pass PRIO_FATAL >= PRIO_TRACE
  BOOST_REQUIRE(mylogger1->fatal());
}

BOOST_AUTO_TEST_CASE(get_level_bad_call) {

  BOOST_TEST_MESSAGE("#Get level bad call (put nonexistent priority)#");

  LoggerPtr mylogger1 = Logger::getLogger("get_level_bad");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // put invalid low priority level
  mylogger1->setLevel(Message::PRIO_TRACE-1);
  // Level must be set to Message::PRIO_INFORMATION
  BOOST_REQUIRE_EQUAL(mylogger1->getLevel(), Message::PRIO_INFORMATION);

  // put invalid high priority level
  mylogger1->setLevel(Message::PRIO_FATAL+1);
  // Level must be set to Message::PRIO_INFORMATION
  BOOST_REQUIRE_EQUAL(mylogger1->getLevel(), Message::PRIO_INFORMATION);
}

BOOST_AUTO_TEST_CASE(get_channel_normal_call) {

  BOOST_TEST_MESSAGE("#Get channel normal call#");
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("get_channel_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  ChannelPtr cc1(new ConsoleChannel(oss));
  BOOST_REQUIRE(cc1);
  // To set a channel
  mylogger1->setChannel(cc1);
  // To get the channel
  ChannelPtr cc2 = mylogger1->getChannel();
  BOOST_REQUIRE(cc2);
}

BOOST_AUTO_TEST_CASE(get_channel_bad_call) {

  BOOST_TEST_MESSAGE("#Get channel bad#");
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("get_channel_bad");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To get the channel
  ChannelPtr cc1 = mylogger1->getChannel();
  BOOST_REQUIRE(!cc1);
}

BOOST_AUTO_TEST_CASE(get_root_logger_normal_call) {

  BOOST_TEST_MESSAGE("#Get root logger normal call#");
  LoggerPtr mylogger1 = Logger::getRootLogger();
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To check that the name of the root logger is empty
  BOOST_REQUIRE(mylogger1->getName().compare("") == 0);
}

BOOST_AUTO_TEST_CASE(create_logger_normal_call) {

  BOOST_TEST_MESSAGE("#Create logger test normal call#");
  // The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  stringstream oss;
  LoggerPtr mylogger1 =
    Logger::createLogger("createLogger_normal",
                         (ChannelPtr) new ConsoleChannel(oss),
                         Message::PRIO_DEBUG);
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To check the name of the logger created
  BOOST_REQUIRE(mylogger1->getName().compare("createLogger_normal") == 0);
  // To check the priority level
  BOOST_REQUIRE(mylogger1->debug());
  // To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
  // To check that the console content is the same that the message to log
  BOOST_REQUIRE(oss.str().compare(msgToLog+"\n") == 0);
}

BOOST_AUTO_TEST_CASE(create_logger_normal_call_default_ConsoleChannel) {

  BOOST_TEST_MESSAGE("#Create logger test normal call on default ConsoleChannel#");
  // The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  LoggerPtr mylogger1 =
    Logger::createLogger("createLogger_normal_default_ConsoleChannel",
                         (ChannelPtr) new ConsoleChannel(),
                         Message::PRIO_DEBUG);
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To check the name of the logger created
  BOOST_REQUIRE(mylogger1->getName().compare("createLogger_normal_default_ConsoleChannel") == 0);
  // To check the priority level
  BOOST_REQUIRE(mylogger1->debug());
  // To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
}

BOOST_AUTO_TEST_CASE(create_logger_normal_call_NullChannel) {

  BOOST_TEST_MESSAGE("#Create logger test normal call on NullChannel#");
  // The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  LoggerPtr mylogger1 =
    Logger::createLogger("createLogger_normal_NullChannel",
                         (ChannelPtr) new NullChannel(),
                         Message::PRIO_DEBUG);
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To check the name of the logger created
  BOOST_REQUIRE(mylogger1->getName().compare("createLogger_normal_NullChannel") == 0);
  // To check the priority level
  BOOST_REQUIRE(mylogger1->debug());
  // To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
}



// BOOST_AUTO_TEST_CASE(create_logger_already_exists_call) {

//   BOOST_TEST_MESSAGE("#Create logger with a name that already exists test#");
//   stringstream oss;
//   // First logger
//   LoggerPtr mylogger1 =
//     Logger::createLogger("createLogger_normal",
//                          (ChannelPtr) new ConsoleChannel(oss),
//                          Message::PRIO_DEBUG);
//   // To check that mylogger1 is not NULL
//   BOOST_REQUIRE(mylogger1);
//   // To check the name of the logger created
//   BOOST_REQUIRE(mylogger1->getName().compare("createLogger_normal") == 0);
//   // To check the priority level
//   BOOST_REQUIRE(mylogger1->debug());

//   // Second logger
//   BOOST_REQUIRE_THROW(Logger::createLogger("createLogger_normal",
//                                            (ChannelPtr) new ConsoleChannel(oss),
//                                            Message::PRIO_DEBUG),
//                       (std::string("Already exists")));

// }


BOOST_AUTO_TEST_CASE(log_message_low_prio_call) {

  BOOST_TEST_MESSAGE("#Log message with low priority test call#");
  // The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  stringstream oss;
  LoggerPtr mylogger1 =
    Logger::createLogger("createLogger_low_prio",
                         (ChannelPtr) new ConsoleChannel(oss),
                         Message::PRIO_FATAL);
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To check the name of the logger created
  BOOST_REQUIRE(mylogger1->getName().compare("createLogger_low_prio") == 0);
  // To check the priority level
  BOOST_REQUIRE(mylogger1->fatal());
  // To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
  // To check that the console content is different from the message
  BOOST_REQUIRE(oss.str().compare(msgToLog+"\n") != 0);
  // To check that the console content is empty
  BOOST_REQUIRE(oss.str().compare("") == 0);
}

BOOST_AUTO_TEST_CASE(log_message_bad_channel_call) {

  BOOST_TEST_MESSAGE("#Log message with bad channel test#");
  // The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  LoggerPtr mylogger1 = Logger::getLogger("get_channel_bad");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  /* Try to log with an empty channel and a priority greater
   * than the one of the logger (Message::PRIO_INFORMATION) */
  mylogger1->log(Message("", msgToLog, Message::PRIO_FATAL));

  /* Try to log with an empty channel and a priority lower
   * than the one of the logger (Message::PRIO_INFORMATION) */
  mylogger1->log(Message("", msgToLog, Message::PRIO_TRACE));
}


BOOST_AUTO_TEST_CASE(destroy_Logger_normal_call) {

  BOOST_TEST_MESSAGE("#Destroy Logger normal call#");
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("destroy_logger_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  Logger::destroyLogger("destroy_logger_normal");
  BOOST_REQUIRE(!Logger::hasLogger("destroy_logger_normal"));
}

BOOST_AUTO_TEST_CASE(shutdown_normal_call) {

  BOOST_TEST_MESSAGE("#Shutdown normal call#");
  stringstream oss;
  std::vector<std::string> names;

  LoggerPtr mylogger1 = Logger::getLogger("shutdown_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To get the list of active Loggers
  Logger::getActiveLoggers(names);
  // To check that the list of Logger's names is not empty
  BOOST_REQUIRE(!names.empty());
  Logger::shutdown();
  Logger::getActiveLoggers(names);
  // To check that the list of Logger's names is empty after shutdown
  BOOST_REQUIRE(names.empty());
}

BOOST_AUTO_TEST_CASE(log_on_file_normal_call) {

  BOOST_TEST_MESSAGE("#FileChannel test normal call#");
  // The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  LoggerPtr mylogger1 = Logger::getLogger("fileChannel_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  ChannelPtr cc1(new FileChannel);
  cc1->putAttr<std::string>("path", temporaryFilename);
  mylogger1->setChannel(cc1);
  // To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
  // To check that the file created is not empty
  BOOST_REQUIRE(((FileChannel*)cc1.get())->getSize() != 0);
  bfs::path file(temporaryFilename);
  // To check that the file with message logged is created
  BOOST_REQUIRE(bfs::exists(file));
  bfs::fstream ifs(file);
  std::string ss;
  std::getline(ifs, ss);
  // To check that the file contains the message to log
  BOOST_REQUIRE(ss.compare(msgToLog) == 0);
  // To remove the file created
  boost::filesystem::remove(file.filename());
}

BOOST_AUTO_TEST_CASE(log_on_console_normal_call) {

  BOOST_TEST_MESSAGE("#ConsoleChannel test normal call#");
  // The message which will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("ConsoleChannel_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  // To put the priority level
  mylogger1->setLevel(Message::PRIO_DEBUG);
  ChannelPtr cc1(new ConsoleChannel(oss));
  mylogger1->setChannel(cc1);
  // To put the message to log
  mylogger1->log(Message("", msgToLog, Message::PRIO_DEBUG));
  // To check that the console content is the same that the message to log
  BOOST_REQUIRE(oss.str().compare(msgToLog+"\n") == 0);
}

BOOST_AUTO_TEST_CASE(get_with_parent_call) {

  BOOST_TEST_MESSAGE("#get with parent test#");
  stringstream oss;
  LoggerPtr mylogger1 = Logger::getLogger("ConsoleChannel_normal");
  // To check that mylogger1 is not NULL
  BOOST_REQUIRE(mylogger1);
  LoggerPtr mylogger2 = Logger::getLogger("ConsoleChannel_normal.second");
  // To check that mylogger2 is not NULL
  BOOST_REQUIRE(mylogger2);

  LoggerPtr mylogger3 = Logger::getLogger("ConsoleChannel_absent.second");
  // To check that mylogger3 is not NULL
  BOOST_REQUIRE(mylogger3);
}

BOOST_AUTO_TEST_SUITE_END()

// THE END

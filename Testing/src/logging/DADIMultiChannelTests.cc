/**
 * @file DADIMultiChannelTests.cc
 * @brief This file implements the libdadi tests for multi channel logger
 * @author Benjamin DEPARDON (benjamin.depardon@sysfera.com)
 */

#include <iostream>
#include <Logging/ConsoleChannel.hh>
#include <Logging/FileChannel.hh>
#include <Logging/NullChannel.hh>
#include <Logging/MultiChannel.hh>
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

BOOST_AUTO_TEST_SUITE(Dadi_MultiChannel_test)

BOOST_AUTO_TEST_CASE(add_remove_channels_test) {

  BOOST_TEST_MESSAGE("#Add and remove channels test#");
  MultiChannel *myMultiC = new MultiChannel();

  // Add two channels
  ChannelPtr myNullC = (ChannelPtr) new NullChannel();
  ChannelPtr myConsoleC = (ChannelPtr) new ConsoleChannel();
  myMultiC->addChannel(myNullC);
  myMultiC->addChannel(myConsoleC);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 2);

  // Remove channels
  myMultiC->removeChannel(myNullC);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 1);
  myMultiC->removeChannel(myConsoleC);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 0);

  // Delete channel
  delete myMultiC;
}

BOOST_AUTO_TEST_CASE(log_channels_test) {

  BOOST_TEST_MESSAGE("#Log on multiple channels test#");
  string source = "Bridgekeeper";
  string msgToLog = "What... is the air-speed velocity of an unladen swallow?";
  Message myMsg = Message(source, msgToLog, Message::PRIO_DEBUG);

  MultiChannel *myMultiC = new MultiChannel();

  // Add two channels
  stringstream oss;
  stringstream oss2;
  ChannelPtr myConsoleC = (ChannelPtr) new ConsoleChannel(oss);
  ChannelPtr myConsoleC2 = (ChannelPtr) new ConsoleChannel(oss2);
  myMultiC->addChannel(myConsoleC);
  myMultiC->addChannel(myConsoleC2);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 2);

  // Log message
  myMultiC->log(myMsg);

  // Check that both console channel received the same message
  BOOST_REQUIRE_EQUAL(oss.str().compare(msgToLog+"\n"), 0);
  BOOST_REQUIRE_EQUAL(oss2.str().compare(msgToLog+"\n"), 0);

  // Delete channel
  delete myMultiC;
}

BOOST_AUTO_TEST_SUITE_END()

// THE END

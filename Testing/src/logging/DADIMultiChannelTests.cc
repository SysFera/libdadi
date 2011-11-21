/**
 * @file DADIMultiChannelTests.cc
 * @brief This file implements the libdadi tests for multi channel logger
 * @author Benjamin DEPARDON (benjamin.depardon@sysfera.com)
 * @section Licence
 *  |LICENCE|
 */

#include <iostream>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <dadi/Logging/ConsoleChannel.hh>
#include <dadi/Logging/FileChannel.hh>
#include <dadi/Logging/NullChannel.hh>
#include <dadi/Logging/MultiChannel.hh>
#include <dadi/Logging/Logger.hh>
#include <dadi/Logging/Message.hh>
#include <dadi/Config.hh>
#include <dadi/Options.hh>

namespace bfs = boost::filesystem;  // an alias for boost filesystem namespace
namespace {
static const std::string temporaryFilename("crap4.log");
typedef boost::scoped_ptr<dadi::MultiChannel> MChannelPtr;
}

BOOST_AUTO_TEST_SUITE(MultiChannelTests)

BOOST_AUTO_TEST_CASE(add_remove_channels_test) {
  BOOST_TEST_MESSAGE("#Add and remove channels test#");
  // ATTENTION: save my nights, use smart pointers (thanks)
  MChannelPtr myMultiC(new dadi::MultiChannel);

  // Add two channels
  dadi::ChannelPtr myNullC(new dadi::NullChannel);
  dadi::ChannelPtr myConsoleC(new dadi::ConsoleChannel);
  myMultiC->addChannel(myNullC);
  myMultiC->addChannel(myConsoleC);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 2);

  // Remove channels
  myMultiC->removeChannel(myNullC);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 1);
  myMultiC->removeChannel(myConsoleC);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 0);
}

BOOST_AUTO_TEST_CASE(log_channels_test) {
  BOOST_TEST_MESSAGE("#Log on multiple channels test#");
  std::string source("Bridgekeeper");
  std::string msgToLog(
    "What... is the air-speed velocity of an unladen swallow?");
  dadi::Message myMsg(source, msgToLog, dadi::Message::PRIO_DEBUG);

  MChannelPtr myMultiC(new dadi::MultiChannel);

  // Add two channels
  std::stringstream oss;
  std::stringstream oss2;
  dadi::ChannelPtr myConsoleC(new dadi::ConsoleChannel(oss));
  dadi::ChannelPtr myConsoleC2(new dadi::ConsoleChannel(oss2));
  myMultiC->addChannel(myConsoleC);
  myMultiC->addChannel(myConsoleC2);
  BOOST_REQUIRE_EQUAL(myMultiC->getCount(), 2);

  // Log message
  myMultiC->log(myMsg);

  // Check that both console channel received the same message
  // FIXME: not elegant at all
  BOOST_REQUIRE_EQUAL(oss.str().compare(msgToLog + "\n"), 0);
  BOOST_REQUIRE_EQUAL(oss2.str().compare(msgToLog + "\n"), 0);
}

BOOST_AUTO_TEST_SUITE_END()

// THE END

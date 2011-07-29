/**
 * @file DADIFileChannelTests.cc
 * @brief This file implements the libdadi tests for file channel logger
 * @author Benjamin DEPARDON (benjamin.depardon@sysfera.com)
 */

#include <iostream>
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
namespace bfs = boost::filesystem;  // an alias for boost filesystem namespace

BOOST_AUTO_TEST_SUITE(Dadi_FileChannel_test)

BOOST_AUTO_TEST_CASE(default_constructor_test) {

  BOOST_TEST_MESSAGE("#Default constructor test#");

  FileChannel *myFileC = new FileChannel();

  // Nothing to test?

  // Delete channel
  delete myFileC;

}


BOOST_AUTO_TEST_CASE(constructor_file_does_not_exist_test) {

  BOOST_TEST_MESSAGE("#Constructor test where file does not exist#");

  // Create working file
  bfs::path tmpFile = bfs::temp_directory_path().native();
  tmpFile /= "%%%%-%%%%-%%%%-%%%%";
  tmpFile = bfs::unique_path(tmpFile).native();
  BOOST_TEST_MESSAGE("tmp file = " + tmpFile.native());

  FileChannel *myFileC = new FileChannel(tmpFile.native());

  // Check correct path
  BOOST_REQUIRE_EQUAL(myFileC->getPath(), tmpFile.native());

  // Check that size is 0
  BOOST_REQUIRE_EQUAL(myFileC->getSize(), 0);

  // Check that last write time is -1 (file does not exist)
  BOOST_REQUIRE_EQUAL(myFileC->getLastWriteTime(), -1);
  
  // Delete channel
  delete myFileC;

  // Delete working file
  bfs::remove_all(tmpFile);
}

BOOST_AUTO_TEST_CASE(constructor_file_exits_test) {

  BOOST_TEST_MESSAGE("#Constructor file exists test#");

  string source = "Bridgekeeper";
  string msgToLog = "What... is the air-speed velocity of an unladen swallow?";
  Message myMsg = Message(source, msgToLog, Message::PRIO_DEBUG);

  // Create working file
  bfs::path tmpFile = bfs::temp_directory_path().native();
  tmpFile /= "%%%%-%%%%-%%%%-%%%%";
  tmpFile = bfs::unique_path(tmpFile).native();
  BOOST_TEST_MESSAGE("tmp file = " + tmpFile.native());

  // Create file
  bfs::fstream ofs(tmpFile, std::ios_base::out);
  ofs << msgToLog;
  ofs.close();

  FileChannel *myFileC = new FileChannel(tmpFile.native());

  // To check that the file with message logged is created
  BOOST_REQUIRE(bfs::exists(tmpFile));

  // Check correct path
  BOOST_REQUIRE_EQUAL(myFileC->getPath(), tmpFile);

  // Check that size is different from 0
  BOOST_REQUIRE(myFileC->getSize() != 0);

  // Check that last write time is different from -1 (file exists)
  BOOST_REQUIRE(myFileC->getLastWriteTime() != -1);
  
  // Delete channel
  delete myFileC;

  // Delete working file
  bfs::remove_all(tmpFile);
}


BOOST_AUTO_TEST_CASE(path_to_directory_test) {

  BOOST_TEST_MESSAGE("#Path to directory test#");

  // Create working directory
  string tmpDir = bfs::unique_path(bfs::temp_directory_path().native()
                                   +  "%%%%-%%%%-%%%%-%%%%").native();
  bfs::create_directory(tmpDir);
  BOOST_TEST_MESSAGE("tmp dir = " + tmpDir);

  FileChannel *myFileC = new FileChannel(tmpDir);

  // Check correct path
  BOOST_REQUIRE_EQUAL(myFileC->getPath(), tmpDir);

  // Check that size is 0
  BOOST_REQUIRE_EQUAL(myFileC->getSize(), 0);

  // Check that last write time is -1
  BOOST_REQUIRE_EQUAL(myFileC->getLastWriteTime(), -1);
  
  // Delete channel
  delete myFileC;

  // Delete working file
  bfs::remove_all(tmpDir);
}


BOOST_AUTO_TEST_SUITE_END()

// THE END

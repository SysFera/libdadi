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
  bfs::path tmpFile = bfs::temp_directory_path();
  tmpFile /= "%%%%-%%%%-%%%%-%%%%";
  tmpFile = bfs::unique_path(tmpFile);
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
  bfs::path tmpDir = bfs::temp_directory_path();
  tmpDir /= "%%%%-%%%%-%%%%-%%%%";
  tmpDir = bfs::unique_path(tmpDir);
  bfs::create_directory(tmpDir);

  BOOST_TEST_MESSAGE("tmp dir = " + tmpDir.native());

  FileChannel *myFileC = new FileChannel(tmpDir.native());

  // FIXME: when FileChannel::open will throw an exception
  // re-enable this test (with the proper exception type
  // Check that open throws an exception
  //BOOST_REQUIRE_THROW( myFileC->open(), std::exception)

  delete myFileC;
  // Delete working file
  bfs::remove_all(tmpDir);
}


BOOST_AUTO_TEST_CASE(open_file_exits_test) {

  BOOST_TEST_MESSAGE("#Open file exists test#");

  string source = "Bridgekeeper";
  string msgToLog = "What... is the air-speed velocity of an unladen swallow?";
  Message myMsg = Message(source, msgToLog, Message::PRIO_DEBUG);

  // Create working directory
  bfs::path tmpDir = bfs::temp_directory_path();
  tmpDir /= "%%%%-%%%%-%%%%-%%%%";
  tmpDir = bfs::unique_path(tmpDir);
  bfs::create_directory(tmpDir);
  BOOST_TEST_MESSAGE("tmp directory = " + tmpDir.native());

  // Create file
  bfs::path tmpFile = tmpDir;
  tmpFile /= "tmpFile.log";
  BOOST_TEST_MESSAGE("tmp file = " + tmpFile.native());

  // Create Channel
  FileChannel *myFileC = new FileChannel(tmpFile.native());

  // Check correct path
  BOOST_REQUIRE_EQUAL(myFileC->getPath(), tmpFile);


  /* We test open for every combination of:
   *  - CompressionMode 
   *  - ArchiveMode
   *  - RotateMode
   *  - PurgeMode
   */
  unsigned int rotateSize = 1024 * 1024; // 1Mo
  string rotateTime = "1:00:00"; // 1h
  string rotateInterval = "1,12:00:00"; // 1 1/2 day
  unsigned int purgeCount = 3; // 3 files

  myFileC->putAttr("rotate.size", rotateSize);
  myFileC->putAttr("rotate.time", rotateTime);
  myFileC->putAttr("rotate.interval", rotateInterval);
  myFileC->putAttr("purge.count", purgeCount);

  for (unsigned int cm = FileChannel::COMP_NONE;
       cm < FileChannel::COMP_ZLIB; ++ cm) {
    // set Compression Mode
    myFileC->putAttr("compression_mode", cm);

    for (unsigned int am = FileChannel::AR_NONE;
         am < FileChannel::AR_TIMESTAMP; ++ am) {
      // set Archive Mode
      myFileC->putAttr("archive", am);

      for (unsigned int rm = FileChannel:: ROT_NONE;
           rm < FileChannel::ROT_LINE; ++ rm) {
        // set Rotate Mode
        myFileC->putAttr("rotate", rm);

        for (unsigned int pm = FileChannel::PURGE_NONE;
             pm < FileChannel::PURGE_AGE; ++ pm) {
          // set Purge Mode
          myFileC->putAttr("purge", pm);
          
          // Log a message
          BOOST_REQUIRE_NO_THROW(myFileC->log(myMsg));

          // To check that the file with message logged is created
          BOOST_REQUIRE(bfs::exists(tmpFile));
          
          // Close Channel
          BOOST_REQUIRE_NO_THROW(myFileC->close());
        }
      }
    }
  }

  // Delete channel
  delete myFileC;

  // Delete working file
  bfs::remove_all(tmpDir);
}


// // List directory
// for (bfs::directory_iterator dirIt = bfs::directory_iterator(tmpDir);
//      dirIt != bfs::directory_iterator(); ++ dirIt) {
//   BOOST_TEST_MESSAGE("File: " + dirIt->path().native());
//  }

BOOST_AUTO_TEST_SUITE_END()

// THE END

/**
 * @file DADIFileChannelTests.cc
 * @brief This file implements the libdadi tests for file channel logger
 * @author Benjamin DEPARDON (benjamin.depardon@sysfera.com)
 * @section Licence
 *  |LICENCE|
 *
 */

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include "Logging/FileChannel.hh"
#include "Logging/Logger.hh"
#include "Logging/Message.hh"
#include "Config.hh"
#include "Options.hh"

namespace bfs = boost::filesystem;  // an alias for boost filesystem namespace

namespace {
typedef boost::scoped_ptr<dadi::FileChannel> FChannelPtr;
}

BOOST_AUTO_TEST_SUITE(FileChannelTests)

BOOST_AUTO_TEST_CASE(default_constructor_test) {
  BOOST_TEST_MESSAGE("#Default constructor test#");
  FChannelPtr myFileC(new dadi::FileChannel);
  // Nothing to test?
}


BOOST_AUTO_TEST_CASE(constructor_file_does_not_exist_test) {
  BOOST_TEST_MESSAGE("#Constructor test where file does not exist#");

  // Create working file
  bfs::path tmpFile = bfs::temp_directory_path().native();
  tmpFile /= "%%%%-%%%%-%%%%-%%%%";
  tmpFile = bfs::unique_path(tmpFile).native();
  BOOST_TEST_MESSAGE("tmp file = " + tmpFile.native());

  FChannelPtr myFileC(new dadi::FileChannel(tmpFile.native()));

  // Check correct path
  BOOST_REQUIRE_EQUAL(myFileC->getPath(), tmpFile.native());
  // Check that size is 0
  BOOST_REQUIRE_EQUAL(myFileC->getSize(), 0);
  // Check that last write time is -1 (file does not exist)
  BOOST_REQUIRE_EQUAL(myFileC->getLastWriteTime(), -1);
  // Delete working file
  bfs::remove_all(tmpFile);
}

BOOST_AUTO_TEST_CASE(constructor_file_exits_test) {
  BOOST_TEST_MESSAGE("#Constructor file exists test#");

  std::string source("Bridgekeeper");
  std::string msgToLog(
    "What... is the air-speed velocity of an unladen swallow?");
  dadi::Message myMsg =
    dadi::Message(source, msgToLog, dadi::Message::PRIO_DEBUG);

  // Create working file
  bfs::path tmpFile = bfs::temp_directory_path();
  tmpFile /= "%%%%-%%%%-%%%%-%%%%";
  tmpFile = bfs::unique_path(tmpFile);
  BOOST_TEST_MESSAGE("tmp file = " + tmpFile.native());

  // Create file
  bfs::fstream ofs(tmpFile, std::ios_base::out);
  ofs << msgToLog;
  ofs.close();

  FChannelPtr myFileC(new dadi::FileChannel(tmpFile.native()));

  // To check that the file with message logged is created
  BOOST_REQUIRE(bfs::exists(tmpFile));
  // Check correct path
  BOOST_REQUIRE_EQUAL(myFileC->getPath(), tmpFile);
  // Check that size is different from 0
  BOOST_REQUIRE(myFileC->getSize() != 0);
  // Check that last write time is different from -1 (file exists)
  BOOST_REQUIRE(myFileC->getLastWriteTime() != -1);
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

  FChannelPtr myFileC(new dadi::FileChannel(tmpDir.native()));

  // FIXME: when dadi::FileChannel::open will throw an exception
  // re-enable this test (with the proper exception type
  // Check that open throws an exception
  // BOOST_REQUIRE_THROW( myFileC->open(), std::exception)
  // Delete working file
  bfs::remove_all(tmpDir);
}


BOOST_AUTO_TEST_CASE(open_file_exits_test) {
  BOOST_TEST_MESSAGE("#Open file exists test#");

  std::string source("Bridgekeeper");
  std::string msgToLog(
    "What... is the air-speed velocity of an unladen swallow?");
  dadi::Message myMsg =
    dadi::Message(source, msgToLog, dadi::Message::PRIO_DEBUG);

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
  FChannelPtr myFileC(new dadi::FileChannel(tmpFile.native()));

  // Check correct path
  BOOST_REQUIRE_EQUAL(myFileC->getPath(), tmpFile);

  /* We test open for every combination of:
   *  - CompressionMode
   *  - ArchiveMode
   *  - RotateMode
   *  - PurgeMode
   */
  std::string rotateSize("2M");
  std::string rotateTime("1:00:00");
  std::string rotateInterval("1,12:00:00");
  unsigned int purgeCount = 3;

  myFileC->putAttr("rotate.size", rotateSize);
  myFileC->putAttr("rotate.time", rotateTime);
  myFileC->putAttr("rotate.interval", rotateInterval);
  myFileC->putAttr("purge.count", purgeCount);

  for (unsigned int cm = dadi::FileChannel::COMP_NONE;
       cm < dadi::FileChannel::COMP_ZLIB; ++cm) {
    // set Compression Mode
    myFileC->putAttr("compression_mode", cm);

    for (unsigned int am = dadi::FileChannel::AR_NONE;
         am < dadi::FileChannel::AR_TIMESTAMP; ++am) {
      // set Archive Mode
      myFileC->putAttr("archive", am);

      for (unsigned int rm = dadi::FileChannel:: ROT_NONE;
           rm < dadi::FileChannel::ROT_LINE; ++rm) {
        // set Rotate Mode
        myFileC->putAttr("rotate", rm);

        for (unsigned int pm = dadi::FileChannel::PURGE_NONE;
             pm < dadi::FileChannel::PURGE_AGE; ++pm) {
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
  // Delete working file
  bfs::remove_all(tmpDir);
}

BOOST_AUTO_TEST_SUITE_END()

// THE END

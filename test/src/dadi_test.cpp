#include <iostream>
//#include <Logging/ConsoleChannel.hh>
#include <Logging/FileChannel.hh>
//#include <Logging/LogServiceChannel.hh>
#include <Logging/Logger.hh>
#include <Logging/Message.hh>

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

BOOST_AUTO_TEST_SUITE (dadi_test)

BOOST_AUTO_TEST_CASE( dadi_normal_call) {

  BOOST_TEST_MESSAGE("Dadi Logger tests");
  //The messagewhich will be logged
  string msgToLog = "Dadi Logger tests: DEBUG";
  try {
    LoggerPtr mylogger1 = Logger::getLogger("test1");
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
    bfs::path file (boost::filesystem3::current_path().string() +"/"+ temporaryFilename);
    //To check that the file with message logged is created
    BOOST_REQUIRE(bfs::exists(file));
    bfs::ifstream ifs (file);
    // Read the whole file into string
    std::stringstream ss;
    ss << ifs.rdbuf();
    //To check that the file contains the message to log
    BOOST_REQUIRE(ss.str().find(msgToLog)!=string::npos);
    //To remove the file created
    boost::filesystem3::remove(file.filename());
  } catch (...) {
    BOOST_MESSAGE("FAILED\n");
  }
}
BOOST_AUTO_TEST_SUITE_END()

// THE END

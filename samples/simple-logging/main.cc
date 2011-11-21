/**
 * @file   samples/simple-logging/main.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  sample program showing how to use logging API
 * @section Licence
 *   |LICENCE|
 *
 */

#include <iostream>
#include <boost/thread.hpp>
#include <dadi/Logging/ConsoleChannel.hh>
#include <dadi/Logging/FileChannel.hh>
#include <dadi/Logging/LogServiceChannel.hh>
#include <dadi/Logging/Logger.hh>
#include <dadi/Logging/Message.hh>

int
main(int argc, char *argv[]) {
  dadi::LoggerPtr mylogger1 = dadi::Logger::getLogger("test1");
  dadi::LoggerPtr mylogger2 = dadi::Logger::getLogger("test1.test2");
  dadi::LoggerPtr mylogger3 = dadi::Logger::getLogger("test1.test3");
  mylogger1->setLevel(dadi::Message::PRIO_DEBUG);
  mylogger2->setLevel(dadi::Message::PRIO_TRACE);
  mylogger3->setLevel(dadi::Message::PRIO_TRACE);
  dadi::ChannelPtr cc1(new dadi::FileChannel);
  dadi::ChannelPtr cc2(new dadi::ConsoleChannel);
  dadi::ChannelPtr cc3(new dadi::LogServiceChannel(argc, argv));
  cc1->putAttr<std::string>("path", "crap4.log");
  cc1->putAttr<std::string>("compression_mode", "none");
  cc1->putAttr<std::string>("archive", "timestamp");
  cc1->putAttr<std::string>("rotate.time", "utc");
  cc1->putAttr<std::string>("rotate", "size");
  cc1->putAttr<std::string>("rotate.size", "1");
  mylogger1->setChannel(cc1);
  mylogger2->setChannel(cc2);
  mylogger3->setChannel(cc3);
  for (int i = 0; i < 10; i++) {
    mylogger1->log(
      dadi::Message("", "test: DEBUG20 test test", dadi::Message::PRIO_DEBUG));
    mylogger1->log(
      dadi::Message("", "test: FATAL test test", dadi::Message::PRIO_FATAL));
    mylogger2->log(
      dadi::Message("", "test: DEBUG test test", dadi::Message::PRIO_DEBUG));
    mylogger2->log(
      dadi::Message("", "test: FATAL test test", dadi::Message::PRIO_FATAL));
    mylogger3->log(
      dadi::Message("", "test: FATAL test test", dadi::Message::PRIO_FATAL));
//    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }

  std::cout << "size: " << ((dadi::FileChannel*)cc1.get())->getSize() << "\n";
  std::cout << "last modified: "
            << ((dadi::FileChannel*)cc1.get())->getLastWriteTime() << "\n";
  dadi::Logger::shutdown();
  return 0;
}


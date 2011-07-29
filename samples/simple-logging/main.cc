#include <iostream>
#include <boost/thread.hpp>
#include <Logging/ConsoleChannel.hh>
#include <Logging/FileChannel.hh>
#include <Logging/LogServiceChannel.hh>
#include <Logging/Logger.hh>
#include <Logging/Message.hh>

using namespace dadi;

int
main(int argc, char *argv[]) {
  LoggerPtr mylogger1 = Logger::getLogger("test1");
  LoggerPtr mylogger2 = Logger::getLogger("test1.test2");
  LoggerPtr mylogger3 = Logger::getLogger("test1.test3");
  mylogger1->setLevel(Message::PRIO_DEBUG);
  mylogger2->setLevel(Message::PRIO_TRACE);
  mylogger3->setLevel(Message::PRIO_TRACE);
  ChannelPtr cc1(new FileChannel);
  ChannelPtr cc2(new ConsoleChannel);
  ChannelPtr cc3(new LogServiceChannel(argc, argv));
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
    mylogger1->log(Message("", "test: DEBUG test test", Message::PRIO_DEBUG));
    mylogger1->log(Message("", "test: FATAL test test", Message::PRIO_FATAL));
    mylogger2->log(Message("", "test: DEBUG test test", Message::PRIO_DEBUG));
    mylogger2->log(Message("", "test: FATAL test test", Message::PRIO_FATAL));
    mylogger3->log(Message("", "test: FATAL test test", Message::PRIO_FATAL));
//    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }

  std::cout << "size: " << ((FileChannel* )cc1.get())->getSize() << "\n";
  std::cout << "last modified: "
            << ((FileChannel* )cc1.get())->getLastWriteTime() << "\n";
  Logger::shutdown();
  return 0;
}


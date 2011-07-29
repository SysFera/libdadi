#include <Logging/LogServiceChannel.hh>
#include <iostream>
#include <boost/thread/locks.hpp>
#include <Logging/Message.hh>

namespace dadi {

typedef boost::lock_guard<boost::mutex> Lock;

LogServiceChannel::LogServiceChannel(int argc, char *argv[]) {
    try {
      LogORBMgr::init(argc, argv);
    } catch (...) {
      std::cerr << "LogORBMgr: initialization failure\n";
      return;
    }
    bool success(false);
    short ret(LS_OK);

    // TODO: get logger name
    lb = new LogComponentBase(&success, argc, argv, 0, "myComponent");

    if (!success) {
      std::cerr << "LogComponentBase: initialization failure\n";
      return;
    }
    /* TODO: needs to be moved in log (requires that LogComponentBase gets
       a shiny new method to test if connection is alive) */
    open();
}

LogServiceChannel::~LogServiceChannel() {
  close();
  delete lb;
}


void
LogServiceChannel::open() {
  Lock lock(mutex_);

  short ret;
  ret = lb->connect("channel connected");
  // TODO: fix this
  switch(ret) {
  case LS_COMPONENT_CONNECT_BADNAME:
  case LS_COMPONENT_CONNECT_ALREADYEXISTS:
  case LS_COMPONENT_CONNECT_BADCOMPONENTCONFIGURATOR:
  case LS_COMPONENT_CONNECT_INTERNALERROR:
    std::cerr << "ERROR: Component connection\n";
    return;
  default:
    break;
  }
}

void
LogServiceChannel::close() {
  Lock lock(mutex_);

  lb->disconnect("channel disconnected");
}

void
LogServiceChannel::log(const Message& msg) {
  Lock lock(mutex_);
  // TODO: implement tags (tags filters on channel side, tags on Message side)
  lb->sendMsg("ANY", msg.getText().c_str());
}


} /* namespace dadi */

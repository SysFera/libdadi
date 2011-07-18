#include <Logging/LogServiceChannel.hh>
#include <Logging/Message.hh>

namespace dadi {

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
    open(); // TODO: needs to be moved in log (requires that LogComponentBase gets a shiny new method to test if connection is alive)
}

LogServiceChannel::~LogServiceChannel() {
  close();
  delete lb;
}


void
LogServiceChannel::open() {
  short ret;
  ret = lb->connect("channel connected");
  // switch(ret) {
  // case LS_COMPONENT_CONNECT_BADNAME:
  // case LS_COMPONENT_CONNECT_ALREADYEXISTS:
  // case LS_COMPONENT_CONNECT_BADCOMPONENTCONFIGURATOR:
  // case LS_COMPONENT_CONNECT_INTERNALERROR:
  //   std::cerr << "ERROR: Component connection\n";
  //   return;
  // default:
  //   break;
  // }
}

void
LogServiceChannel::close() {
  lb->disconnect("channel disconnected");
}

void
LogServiceChannel::log(const Message& msg) {
  std::cout << "message: " << msg.getText() << "\n";
  // TODO: implement tags (tags filters on channel side, tags on Message side)
  lb->sendMsg("ANY", msg.getText().c_str());
}


} /* namespace dadi */

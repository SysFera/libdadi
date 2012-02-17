/**
 * @file   samples/simple-cori/main.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  sample program showing how to use cori API
 * @section Licence
 *   |LICENCE|
 *
 */

#include <list>
#include <string>
#include <vector>
#include <boost/foreach.hpp>
#include <dadi/Config.hh>
#include <dadi/CoriMgr.hh>
#include <dadi/ICori.hh>
#include <dadi/Loader.hh>
#include <dadi/Options.hh>
#include <dadi/Registry.hh>

int
main(int argc, char *argv[]) {
  typedef std::vector<std::string> StringVec;

  dadi::Options opt;
  dadi::Config& config = dadi::Config::instance();

  opt.setName("simple-cori");
  opt.setUsage("[-h] [options] <paths to plugins manifests>");
  boost::function1<void, StringVec> fPaths(
    boost::bind(dadi::setPropertySeq<StringVec>, "paths.path", _1));
  opt.addOption("paths,p", "path", fPaths);
  opt.setPositional("paths.path");
  opt.parseCommandLine(argc, argv);
  opt.notify();

  StringVec c = config.get_seq<StringVec>("paths");

  dadi::Registry& reg = dadi::Registry::instance();
  BOOST_FOREACH(const std::string& p, c) {
    reg.addPath(p);
  }
  reg.load();
  dadi::CoriMgr mgr;
  std::cout << "===== Cori plugins loaded ====\n";
  std::list<std::string> l = mgr.listPlugins();
  BOOST_FOREACH(std::string& v, l) {
    std::cout << v << "\n";
  }
  std::cout << "==============================\n\n";

  std::cout << "====== Metrics available =====\n"
            << dadi::str(mgr.listMetrics())
            << "\n=============================\n\n";

  std::cout << "========== Results ==========\n"
            << dadi::str(mgr.getMetrics("<diet><cori><metrics>"
                                        "<metric>uptime</metric>"
                                        "<metric>ram.total</metric>"
                                        "<metric>swap.total</metric>"
                                        "</metrics></cori></diet>"))
            << "=============================\n";

  return 0;
}

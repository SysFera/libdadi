/**
 * @file DADIConfigFileTestsFixtures.hh
 * @brief ConfigFile tests fixtures
 * @author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 * @section Licence
 *   |LICENCE|
 *
 */
#include <iostream>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>
#include <dadi/Config.hh>
#include <dadi/ConfigMgr.hh>
#include <dadi/Options.hh>
#include "testconfig.h"

class ConfLoader {
public:
  ConfLoader() {
    BOOST_TEST_MESSAGE("[config Loader] LOAD CONFIG BEGIN");
    dadi::Config& config = dadi::Config::instance();

    boost::filesystem::path configFilepath(TESTFILESOUTPUTPATH);
    configFilepath /= "cfg";
    configFilepath /= "infoConfigFile.cfg";
    {
      std::ifstream ifs(configFilepath.native().c_str());
      config.load(ifs);
    }

    BOOST_TEST_MESSAGE("[config Loader] LOAD CONFIG END");
  }

  ~ConfLoader() {}
};

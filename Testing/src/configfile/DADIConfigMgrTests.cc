/**
 * @file DADIConfigMgrTests.cc
 * @brief This file implements the libdadi tests for configFile
 * @author Ibrahima Cissé (ibrahima.cisse@sysfera.com
 * @section License
 *  |LICENSE|
 *
 */

#include "DADIConfigTestsFixtures.hh"

namespace bfs = boost::filesystem;  // an alias for boost filesystem namespace

BOOST_AUTO_TEST_SUITE(ConfigMgrTests)

BOOST_AUTO_TEST_CASE(configMgr_dagda) {
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
  // get the dagda config manager
  boost::shared_ptr<dadi::ConfigMgr> dagdaConfigMgr(
    dadi::getConfigMgr("diet.dagda"));

  BOOST_CHECK_EQUAL(dagdaConfigMgr->getRootKey(), "diet.dagda");
  std::vector<std::string> StoragesNames(
    dagdaConfigMgr->get<std::string>("Storages", "name"));
  std::vector<std::string> StoragesTypes(
    dagdaConfigMgr->get<std::string>("Storages", "type"));
  std::vector<unsigned> StoragesSizes(
    dagdaConfigMgr->get<unsigned>("Storages", "size"));
  BOOST_CHECK((StoragesNames.size() == StoragesTypes.size()) &&
              (StoragesTypes.size()== StoragesSizes.size()));
}

/**************************************************************/

BOOST_AUTO_TEST_CASE(configMgr_core) {
  BOOST_TEST_MESSAGE("[config Loader] LOAD CONFIG BEGIN");
  dadi::Config& config = dadi::Config::instance();

  boost::filesystem::path configFilepath(TESTFILESOUTPUTPATH);
  configFilepath /= "cfg";
  configFilepath /= "infoConfigFile.cfg";
  {
    std::ifstream ifs(configFilepath.native().c_str());
    config.load(ifs);
  }

  // get the core config manager
  boost::shared_ptr<dadi::ConfigMgr> dagdaConfigMgr(
    dadi::getConfigMgr("diet.core"));
  BOOST_CHECK_EQUAL(dagdaConfigMgr->getRootKey(), "diet.core");
  std::vector<std::string> usersNames(
    dagdaConfigMgr->get<std::string>("users", "name"));
  std::vector<unsigned> usersQuotas(
    dagdaConfigMgr->get<unsigned>("users", "quotas"));
  BOOST_REQUIRE_EQUAL(usersNames.size(), usersQuotas.size());

  BOOST_FOREACH(std::string name, usersNames) {
    BOOST_TEST_MESSAGE("name: " << name);
  }
}

BOOST_AUTO_TEST_SUITE_END()

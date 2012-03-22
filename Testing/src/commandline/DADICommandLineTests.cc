/**
 * @file DADICommandLineTests.cc
 * @brief This file implements the libdadi tests for configFile
 * @author Haikel Guémar <haikel.guemar@sysfera.com>
 * @section License
 *  |LICENSE|
 *
 */

#include <boost/test/unit_test.hpp>
#include <boost/program_options.hpp>
#include "dadi/Config.hh"
#include "dadi/Options.hh"

namespace po = boost::program_options;

BOOST_AUTO_TEST_SUITE(CommandLineTests)

BOOST_AUTO_TEST_CASE(required_option_exception) {
  BOOST_TEST_MESSAGE("#Test required option exception raised#");

  int argc = 2;
  char *argv[argc + 1];

  dadi::Options opt;
  dadi::Config& config = dadi::Config::instance();

  boost::function1<void, std::string> fRequired(
    boost::bind(dadi::setPropertyString, "test", _1));

  opt.addOption("required,r", "required option test", fRequired, true);
  opt.addSwitch("unrequired,u", "unrequired option test");

  argv[0] = const_cast<char *>("./test");
  argv[1] = const_cast<char *>("-u");
  opt.parseCommandLine(argc, argv);
  BOOST_CHECK_THROW(opt.notify(), po::required_option);
}

BOOST_AUTO_TEST_SUITE_END()

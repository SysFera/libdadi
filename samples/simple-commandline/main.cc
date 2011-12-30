/**
 * @file   samples/simple-commandline/main.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  sample program showing how to use command-line file API
 * @section Licence
 *   |LICENCE|
 *
 */
#include <cstdlib>
#include <iostream>
// force debug traces in dadi::setProperty
#define NDEBUG

#include <dadi/Config.hh>
#include <dadi/Options.hh>

void
my_help(const dadi::Options& opt);


int
main(int argc, char *argv[], char *envp[]) {
  dadi::Options opt;
  dadi::Config& config = dadi::Config::instance();

  opt.setName("simple-commandline");
  opt.setUsage("[-h] [options] -n <name> -j <number>");
  boost::function0<void> fHelp(boost::bind(my_help, boost::cref(opt)));
  boost::function1<void, std::string> fName(
    boost::bind(dadi::setPropertyString, "name", _1));
  boost::function1<void, int> fJobs(
    boost::bind(dadi::setProperty<int>, "jobs", _1));

  opt.addSwitch("help,h", "display help message", fHelp);
  opt.addOption("name,n", "name", fName);
  opt.addOption("jobs,j", "number of jobs", fJobs);

  opt.parseCommandLine(argc, argv);
  opt.notify();

  return EXIT_SUCCESS;
}

void
my_help(const dadi::Options& opt) {
  std::cout << "[my_help callback]\n";
  dadi::help(opt);
}

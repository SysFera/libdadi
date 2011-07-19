#include <cstdlib>
#include <iostream>

#include "config.hh"
#include "options.hh"

/* for testing purpose, you need a configuration file formatted like this:

   [file]
   name=<xxxx>

*/



int main(int argc, char *argv[], char *envp[]) {
  dadi::Options opt;
  dadi::Config& config = dadi::Config::instance();

  opt.setName("simple-configfile");
  opt.setUsage("-c <config-file>");

  // we prefix command line and file options respectively with command and file
  /* command line options */
  boost::function0<void> fHelp(boost::bind(dadi::help, boost::cref(opt)));
  boost::function1<void, std::string> fFile(
    boost::bind(dadi::setPropertyString, "command.file", _1));
  /* configuration file options */
  boost::function1<void, std::string> fName(
    boost::bind(dadi::setPropertyString, "file.name", _1));
  dadi::OptionsGroup fileGroup("file configuration", true,
                               dadi::OptionsGroup::CFG);
  fileGroup.addOption("file.name", "name", fName);

  opt.addSwitch("help,h", "display help message", fHelp);
  // configuration file is a MANDATORY option
  opt.addOption("config-file,c", "configuration file", fFile)->required();
  opt.addGroup(fileGroup);

  // first parse command line
  opt.parseCommandLine(argc, argv);
  opt.notify();

  // then get configuration file path
  std::string cfile = boost::any_cast<std::string>(config["command.file"]);

  // finally parse configuration file
  // if you want to allow unregistered options, set second arg to true
  try {
    opt.parseConfigFile(cfile, false);
  }
  catch (...) {
    std::cerr << "Unrecognized configuration file\n";
    return EXIT_FAILURE;
  }
  opt.notify();

  if (!config["file.name"].empty()) {
    std::cout << "==== file ====\n"
              << "file.name: "
              << boost::any_cast<std::string>(config["file.name"])
              << "\n";
  } else {
    std::cerr << "empty key: file.name\n";
  }

  return EXIT_SUCCESS;
}



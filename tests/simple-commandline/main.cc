#include <cstdlib>
#include <iostream>

#include "config.hh"
#include "options.hh"


int main(int argc, char *argv[], char *envp[]) {
  dadi::Options opt;
  dadi::Config& config = dadi::Config::instance();

  opt.setName("cline-1");
  boost::function0<void> fHelp(boost::bind(dadi::help, boost::cref(opt)));

  opt.addSwitch("help,h", "display help message", fHelp);

  opt.parseCommandLine(argc, argv);
  opt.notify();

  return EXIT_SUCCESS;
}


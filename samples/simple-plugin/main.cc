/**
 * @file   samples/simple-plugin/main.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  sample program showing how to use plugin API
 * @section License
 *   |LICENSE|
 *
 */
#include <cstdlib>
#include <iostream>

#include <dadi/Logging/IPerson.hh>
#include <dadi/Loader.hh>
#include <dadi/Registry.hh>


int
main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "usage: loader <plugin_config>\n";
    return EXIT_FAILURE;
  }

  std::cout << "[LOADER]: start\n";
  dadi::Registry reg;
  int i = 1;
  while (argv[i]) {
    reg.addPath(argv[i++]);
  }
  try {
    reg.load();
  } catch (...) {
    std::cerr << "Directory loading error\n";
    return EXIT_FAILURE;
  }
  IPerson *p = reg.getByInterface<IPerson>("IPerson");
  if (p) {
    p->init();
    p->say_hello();
  } else {
    std::cout << "Failed to load Plugin PersonA\n";
  }

  std::cout << "[LOADER]: stop\n";
  return EXIT_SUCCESS;
}

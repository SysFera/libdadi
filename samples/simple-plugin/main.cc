#include <cstdlib>
#include <iostream>

#include "IPerson.hh"
#include "Loader.hh"
#include "Registry.hh"


int main(int argc, char *argv[]){
  if (argc < 2) {
    std::cout << "usage: loader <plugin_config>\n";
    return EXIT_FAILURE;
  }

  std::cout << "[LOADER]: start\n";
  dadi::Registry reg;
  int i = 1;
  while(argv[i]) {
    reg.addPath(argv[i++]);
  }

  reg.load();

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

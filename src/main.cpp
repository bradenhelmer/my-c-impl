#include <iostream>

#include "Common.h"
#include "Driver.h"
#include "Token.h"

int main(int argc, char **argv) {
  ASSERT(argc > 2);
  const std::string option(argv[1]);
  const std::string file(argv[2]);
  Driver driver(file);
  if (!option.compare("--print")) {
    driver.printAST();
  } else if (!option.compare("--exec")) {
    driver.executeToolchain();
  } else {
    exit(1);
  }
  return 0;
}

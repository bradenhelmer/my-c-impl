#include <iostream>

#include "Common.h"
#include "Driver.h"
#include "Token.h"

int main(int argc, char **argv) {
  ASSERT(argc > 1);
  const std::string file(argv[1]);
  Driver driver(file);
  driver.executeToolchain();
  return 0;
}

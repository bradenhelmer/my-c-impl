// Driver.h
// ~~~~~~~~
// Driver definition
#ifndef DRIVER_H
#define DRIVER_H
#include <string>
#include <vector>

class Driver {
  const std::string fileName;

 private:
  std::vector<char> processInputFile();

 public:
  Driver(const std::string fileName) : fileName(fileName) {}
  void executeToolchain();
};

#endif  // DRIVER_H

// Driver.h
// ~~~~~~~~
// Driver definition
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

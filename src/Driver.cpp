// Driver.cpp
// ~~~~~~~~~~
// Driver implementation.
#include "Driver.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

#include "Common.h"

std::vector<char> Driver::processInputFile() {
  std::ifstream sourceFile(fileName);
  ASSERT(sourceFile.is_open());

  std::vector<char> source;
  std::copy(std::istream_iterator<char>(sourceFile),
            std::istream_iterator<char>(), std::back_inserter(source));
  sourceFile.close();
  return source;
}

void Driver::executeToolchain() {
  std::vector<char> source = processInputFile();
  for (auto it = source.begin(); it != source.end(); ++it) {
    SIMPLE_PRINT(*it);
  }
}

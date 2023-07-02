// Driver.cpp
// ~~~~~~~~~~
// Driver implementation.

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

#include "Common.h"
#include "Driver.h"
#include "Lexer.h"

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
  std::vector<char> buf = processInputFile();
  Lexer lexer(&buf);
  lexer.lexAndPrintTokens();
}

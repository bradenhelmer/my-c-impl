/// Driver.cpp
// ~~~~~~~~~~
// Driver implementation.

#include <cassert>
#include <filesystem>
#include <fstream>

#include "Common.h"
#include "Driver.h"
#include "Lexer.h"
#include "Token.h"

std::vector<char> Driver::processInputFile() {
  std::ifstream sourceFile(fileName);
  ASSERT(sourceFile.is_open());

  char curr;
  std::vector<char> source;
  while (sourceFile.get(curr)) {
    source.push_back(curr);
  }
  sourceFile.close();
  return source;
}

void Driver::executeToolchain() {
  std::vector<char> buf = processInputFile();
  Lexer lexer(&buf);
  lexer.lexAndPrintTokens();
}

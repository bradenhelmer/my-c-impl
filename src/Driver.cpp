/// Driver.cpp
// ~~~~~~~~~~
// Driver implementation.

#include "Driver.h"

#include <filesystem>
#include <fstream>

#include "Common.h"
#include "Lexer.h"
#include "Parser.h"

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
  Parser parser(lexer);
  std::unique_ptr<Program> ast = parser.parseProgram();
}

void Driver::printAST() {
  std::vector<char> buf = processInputFile();
  Lexer lexer(&buf);
  Parser parser(lexer);
  std::unique_ptr<Program> ast = parser.parseProgram();
  ast->print(0);
}

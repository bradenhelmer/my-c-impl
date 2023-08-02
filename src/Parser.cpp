// Parser.cpp
// ~~~~~~~~~~
// Parser implementation
#include "Parser.h"

#include "Common.h"

std::unique_ptr<Program> Parser::parseProgram() {
  // Advance current token to begin parsing.
  advanceCurrent();

  // Parse
  return std::make_unique<Program>(parseDeclList());
}

void Parser::advanceCurrent() {
  lex.advanceToken();
  currTok = lex.getCurrentToken();
}

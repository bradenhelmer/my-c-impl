// Parser.cpp
// ~~~~~~~~~~
// Parser implementation
#include "Parser.h"

#include <memory>

#include "Common.h"
#include "Token.h"

std::unique_ptr<Program> Parser::parseProgram() {
  // Advance current token to begin parsing.
  advanceCurrent();
  return std::make_unique<Program>(parseDeclList());
}

void Parser::advanceCurrent() {
  lex.advanceToken();
  currTok = lex.getCurrentToken();
}

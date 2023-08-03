// Parser.cpp
// ~~~~~~~~~~
// Parser implementation
#include "Parser.h"

#include "Common.h"

std::shared_ptr<Program> Parser::parseProgram() {
  // Advance current token to begin parsing.
  advanceCurrent();
  // Parse
  currentProgram->attachDecls(parseDeclList());
  return currentProgram;
}

void Parser::advanceCurrent() {
  lex.advanceToken();
  currTok = lex.getCurrentToken();
}

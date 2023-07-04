// Parser.cpp
// ~~~~~~~~~~
// Parser implementation
#include "Parser.h"

std::unique_ptr<ExprAST> Parser::parseProgram() {
  lex.advanceToken();
  return parseExpression();
}

// Parser.h
// --------
// Parser definiton
#ifndef PARSER_H
#define PARSER_H
#include <memory>

#include "Lexer.h"
#include "ast.h"

class Parser {
  Lexer lex;

public:
  Parser(Lexer &lex) : lex(lex) {}
  std::unique_ptr<ExprAST> ParseProgram();

private:
};

#endif // PARSER_H

// Parser.h
// --------
// Parser definiton
#ifndef PARSER_H
#define PARSER_H
#include <memory>

#include "Ast.h"
#include "Lexer.h"
#include "Token.h"

class Parser {
  Lexer lex;
  Token *currTok;

 public:
  Parser(Lexer &lex) : lex(lex) {}
  std::unique_ptr<AstNode> parseProgram();
  template <typename T>
  static std::unique_ptr<T> LogError(const char *str);

 private:
  void advanceCurrent();
  std::unique_ptr<BinaryExprAST> parseBinaryExpression(
      int precedence, std::unique_ptr<ExprAST> left);
  std::unique_ptr<ExprAST> parseExpression();
  std::unique_ptr<ExprAST> parseIdentifierExpr();
  std::unique_ptr<NumberExprAST> parseNumberExpr();
  std::unique_ptr<DeclAST> parseDeclaration();
  std::unique_ptr<PrototypeAST> parseProtoType();
  std::map<std::string, TokenKind> parseArgs();
};

#endif  // PARSER_H

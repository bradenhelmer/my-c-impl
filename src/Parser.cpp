// Parser.cpp
// ~~~~~~~~~~
// Parser implementation
#include "Parser.h"

#include <memory>

#include "Common.h"
#include "Token.h"

std::unique_ptr<AstNode> Parser::parseProgram() {
  advanceCurrent();
  switch (currTok->kind) {
    case identifier:
      return parseIdentifierExpr();
    case kwt_int:
    case kwt_char:
      return parseDeclaration();
    default:
      return LogError<AstNode>("Error Parsing!");
  }
}

template <typename T>
std::unique_ptr<T> Parser::LogError(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  return nullptr;
}

void Parser::advanceCurrent() {
  lex.advanceToken();
  currTok = lex.getCurrentToken();
}

std::unique_ptr<BinaryExprAST> Parser::parseBinaryExpression(
    int precedence, std::unique_ptr<ExprAST> left) {}

std::unique_ptr<ExprAST> Parser::parseExpression() {
  std::unique_ptr<ExprAST> result;
  switch(currTok->kind) {
    case numeric_literal:
      return parseNumberExpr();
  }
}
std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
  const std::string idStr = lex.getIdentifier();
  advanceCurrent();
  switch (currTok->kind) {
    case equals:
      break;
    default:
      return LogError<ExprAST>("Error parsing identifier expression.");
  }
}

std::unique_ptr<DeclAST> Parser::parseDeclaration() {
  const TokenKind type = currTok->kind;
  advanceCurrent();
  if (currTok->kind != identifier)
    return LogError<DeclAST>("Expected Identifier");
  const std::string idStr = lex.getIdentifier();
  advanceCurrent();
  switch (currTok->kind) {
    case semi_colon:
      advanceCurrent();
      return std::make_unique<VariableDeclAST>(type, idStr);
    case equals:
      advanceCurrent();
      return std::make_unique<VariableDeclInitAST>(type, idStr,
                                                   parseExpression());
    case o_paren:
      advanceCurrent();
      return parseProtoType();
    default:
      return LogError<DeclAST>("Error parsing declaration.");
  }
}

std::unique_ptr<NumberExprAST> Parser::parseNumberExpr() {
  ASSERT(currTok->kind == numeric_literal);
  auto result = std::make_unique<NumberExprAST>(lex.getNumericLiteral());
  advanceCurrent();
  return result;
}

std::unique_ptr<PrototypeAST> Parser::parseProtoType() {}

std::map<std::string, TokenKind> Parser::parseArgs() {
  std::map<std::string, TokenKind> args;
  while (currTok->kind != c_paren) {
    if (!isTypeKeyword(currTok->kind)) {
      LogError<AstNode>(
          "Unknown type encountered when parsing fucntion arguments.");
      return args;
    }
    TokenKind type = currTok->kind;
    advanceCurrent();
    if (currTok->kind == identifier) {
      std::string name = lex.getIdentifier();
      args.insert({name, type});
      advanceCurrent();
      if (currTok->kind == comma) {
        advanceCurrent();
        continue;
      }
    }
  }
  return args;
}

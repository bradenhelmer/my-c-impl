// Expression Parsing Imlpementations
#include "Ast.h"
#include "Parser.h"

std::unique_ptr<ExprAST> Parser::parsePrimary() {
  switch (currKind()) {
    case num_const:
      return parseNumberExpr();
    case identifier:
      return parseIdentifierExpr();
    case o_paren:
      return parseParentheseExpr();
    default:
      return LogError<ExprAST>("Unknown token when parsing expression!");
  }
}

std::unique_ptr<NumConstAST> Parser::parseNumberExpr() {
  return std::make_unique<NumConstAST>(lex.getNumericLiteral());
}

std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {}

std::unique_ptr<ExprAST> Parser::parseParentheseExpr() {}

std::unique_ptr<ExprAST> Parser::parseExpr() {
  std::unique_ptr<ExprAST> LHS = parsePrimary();
  advanceCurrent();
  if (currKind() == semi_colon) {
    advanceCurrent();
    return LHS;
  }
  return parseBinaryOpExpr(std::move(LHS), getOperatorPrecedence(currKind()));
}

std::unique_ptr<ExprAST> Parser::parseBinaryOpExpr(std::unique_ptr<ExprAST> LHS,
                                                   Precedence prec) {}

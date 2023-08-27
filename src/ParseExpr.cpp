// Expression Parsing Imlpementations
#include "Ast.h"
#include "Parser.h"

std::unique_ptr<NumConstAST> Parser::parseNumberExpr() {
  double val = lex.getNumericLiteral();
  advanceCurrent();
  return std::make_unique<NumConstAST>(getCurrProgramPtr(), val);
}

std::unique_ptr<CharConstAST> Parser::parseCharExpr() {
  char char_const = lex.lexCharConstant();
  if (currKind() != apost)
    return LogError<CharConstAST>("Missing closing apostrophe!");
  advanceCurrent();
  return std::make_unique<CharConstAST>(getCurrProgramPtr(), char_const);
}

std::unique_ptr<StringLiteralAST> Parser::parseStrLiteralExpr() {
  std::string str_literal = lex.lexStringLiteral();
  if (currKind() != quote)
    return LogError<StringLiteralAST>("Missing closing quote!");
  advanceCurrent();
  return std::make_unique<StringLiteralAST>(getCurrProgramPtr(), str_literal);
}

std::unique_ptr<CallExprAST> Parser::parseCallExpr(Identifier &id) {
  std::vector<std::unique_ptr<ExprAST>> args;
  while (currKind() != c_paren) {
    if (currKind() == comma) advanceCurrent();
    args.push_back(parseExpr());
  }
  advanceCurrent();
  return std::make_unique<CallExprAST>(getCurrProgramPtr(), id.idStr,
                                       std::move(args));
}
std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
  Identifier currId = lex.getIdentifier();
  if (currKind() == o_paren) {
    advanceCurrent();
    return parseCallExpr(currId);
  }
  return std::make_unique<VarExprAST>(getCurrProgramPtr(), currId.idStr);
}

std::unique_ptr<ExprAST> Parser::parseParentheseExpr() {
  advanceCurrent();
  std::unique_ptr<ExprAST> V = parseExpr();
  if (!V) return nullptr;
  if (currKind() != c_paren)
    return LogError<ExprAST>("Expected closing parenthese!");
  advanceCurrent();
  return V;
}

std::unique_ptr<ExprAST> Parser::parsePrimaryExpr() {
  switch (currKind()) {
    case num_const:
      return parseNumberExpr();
    case identifier:
      return parseIdentifierExpr();
    case o_paren:
      return parseParentheseExpr();
    case apost:
      return parseCharExpr();
    case quote:
      return parseStrLiteralExpr();
    case semi_colon:
      return nullptr;
    default:
      return LogError<ExprAST>("Unknown token when parsing expression!");
  }
}

std::unique_ptr<ExprAST> Parser::parseExpr() {
  std::unique_ptr<ExprAST> LHS = parsePrimaryExpr();
  return parseBinaryOpExpr(std::move(LHS), base);
}

std::unique_ptr<ExprAST> Parser::parseBinaryOpExpr(std::unique_ptr<ExprAST> LHS,
                                                   Precedence prec) {
  Precedence currPrec = getOperatorPrecedence(currKind());
  while (true) {
    if (currPrec < prec) return LHS;

    TokenKind op = currKind();
    advanceCurrent();

    std::unique_ptr<ExprAST> RHS = parsePrimaryExpr();
    if (!RHS) return nullptr;
    Precedence prevPrec = currPrec;
    currPrec = getOperatorPrecedence(currKind());

    // Exponential expressions are right associative
    bool isRightAssoc = prevPrec == exponential;

    if (currPrec < prevPrec) {
      RHS = parseBinaryOpExpr(
          std::move(RHS), static_cast<Precedence>(prevPrec + !isRightAssoc));
      if (!RHS) return nullptr;
    }
    LHS = std::make_unique<BinaryExprAST>(getCurrProgramPtr(), op,
                                          std::move(LHS), std::move(RHS));
  }
}

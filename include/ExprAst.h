// Expression Abstract Syntax Trees
#ifndef EXPR_AST_H
#define EXPR_AST_H
#include "Ast.h"

class NumConstAST : public ExprAST {
  double numConst;

 public:
  NumConstAST(double value) : numConst(value) {}
  virtual void print(int indenation) const override;
  virtual std::string getTypeString() const override { return "NumConst"; }
};

class CharConstAST : public ExprAST {
  char charConst;

 public:
  CharConstAST(char charConst) : charConst(charConst) {}
};

class StringLiteralAST : public ExprAST {
  const std::string strLiteral;
  double length;

 public:
  StringLiteralAST(const std::string &strLiteral) : strLiteral(strLiteral) {
    length = strLiteral.size();
  }
};
class BinaryExprAST : public ExprAST {
  TokenKind op;
  std::unique_ptr<ExprAST> LHS, RHS;

  BinaryExprAST(TokenKind op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};
class CallExprAST : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

 public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
      : callee(callee), args(std::move(args)) {}
};
class UnaryExprAST;

#endif  // EXPR_AST_H

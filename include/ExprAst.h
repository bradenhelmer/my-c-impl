// Expression Abstract Syntax Trees
#ifndef EXPR_AST_H
#define EXPR_AST_H
#include "Ast.h"

class NumConstAST : public ExprAST {
  double numConst;

 public:
  NumConstAST(double value) : numConst(value) {}
  void print(int indentation) const override;
  std::string getTypeString() const override { return "NumConst"; }
};

class CharConstAST : public ExprAST {
  char charConst;

 public:
  CharConstAST(char charConst) : charConst(charConst) {}
  std::string getTypeString() const override { return "CharConst"; }
  void print(int indentation) const override;
};

class StringLiteralAST : public ExprAST {
  const std::string strLiteral;
  double length;

 public:
  StringLiteralAST(const std::string &strLiteral) : strLiteral(strLiteral) {
    length = strLiteral.size();
  }
  std::string getTypeString() const override { return "StrLiteral"; }
  void print(int indentation) const override;
};

class BinaryExprAST : public ExprAST {
  TokenKind op;
  std::unique_ptr<ExprAST> LHS, RHS;

 public:
  BinaryExprAST(TokenKind op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  std::string getTypeString() const override { return "BinOpExp"; }
  void print(int indentation) const override;
};

class VarExprAST : public ExprAST {
  std::string name;

 public:
  VarExprAST(const std::string &name) : name(name) {}
  std::string getTypeString() const override { return "VarExpr"; }
  void print(int indentation) const override;
  std::string constructVarString() const {
    std::string varStr = getTypeString() + ": " + name;
    return varStr;
  }
};

class CallExprAST : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

 public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
      : callee(callee), args(std::move(args)) {}
  void print(int indentation) const override;
  std::string getTypeString() const override { return "CallExpr"; }
  std::string constructCallStr() const {
    std::string callStr = getTypeString() + " -> " + '\'' + callee + '\'';
    return callStr;
    ;
  }
};

class UnaryExprAST;

#endif  // EXPR_AST_H

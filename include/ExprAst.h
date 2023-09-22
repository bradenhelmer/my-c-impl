// Expression Abstract Syntax Trees
#ifndef EXPR_AST_H
#define EXPR_AST_H
#include "Ast.h"

class NumConstAST : public ExprAST {
  double numConst;
  std::shared_ptr<Program> programRoot;

 public:
  NumConstAST(std::shared_ptr<Program> programRoot, double value)
      : programRoot(programRoot), numConst(value) {}
  void print(int indentation) const override;
  std::string getTypeString() const override { return "NumConst"; }
  llvm::Value *codeGen() override;
};

class CharConstAST : public ExprAST {
  char charConst;
  std::shared_ptr<Program> programRoot;

 public:
  CharConstAST(std::shared_ptr<Program> programRoot, char charConst)
      : programRoot(programRoot), charConst(charConst) {}
  std::string getTypeString() const override { return "CharConst"; }
  void print(int indentation) const override;
  llvm::Value *codeGen() override;
};

class StringLiteralAST : public ExprAST {
  const std::string strLiteral;
  double length;
  std::shared_ptr<Program> programRoot;

 public:
  StringLiteralAST(std::shared_ptr<Program> programRoot,
                   const std::string &strLiteral)
      : programRoot(programRoot), strLiteral(strLiteral) {
    length = strLiteral.size();
  }
  std::string getTypeString() const override { return "StrLiteral"; }
  void print(int indentation) const override;
  llvm::Value *codeGen() override;
};

class BinaryExprAST : public ExprAST {
  TokenKind op;
  std::unique_ptr<ExprAST> LHS, RHS;
  std::shared_ptr<Program> programRoot;

 public:
  BinaryExprAST(std::shared_ptr<Program> programRoot, TokenKind op,
                std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
      : programRoot(programRoot),
        op(op),
        LHS(std::move(LHS)),
        RHS(std::move(RHS)) {}
  std::string getTypeString() const override { return "BinOpExp"; }
  void print(int indentation) const override;
  llvm::Value *codeGen() override;
  llvm::Value *codeGenAssignment();
};

class VarExprAST : public ExprAST {
  std::string name;
  std::shared_ptr<Program> programRoot;

 public:
  VarExprAST(std::shared_ptr<Program> programRoot, const std::string &name)
      : programRoot(programRoot), name(name) {}
  std::string getTypeString() const override { return "VarExpr"; }
  void print(int indentation) const override;
  std::string constructVarString() const {
    std::string varStr = getTypeString() + ": " + name;
    return varStr;
  }
  llvm::Value *codeGen() override;
};

class CallExprAST : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;
  std::shared_ptr<Program> programRoot;

 public:
  CallExprAST(std::shared_ptr<Program> programRoot, const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
      : programRoot(programRoot), callee(callee), args(std::move(args)) {}
  void print(int indentation) const override;
  std::string getTypeString() const override { return "CallExpr"; }
  std::string constructCallStr() const {
    std::string callStr = getTypeString() + " -> " + '\'' + callee + '\'';
    return callStr;
    ;
  }
  llvm::Value *codeGen() override;
};

class UnaryExprAST;

#endif  // EXPR_AST_H

// Statement Abstract Syntax Trees
#ifndef STMT_AST_H
#define STMT_AST_H
#include "Ast.h"

class BlockStmtAST : public StmtAST {
  std::vector<std::unique_ptr<StmtAST>> stmtList;

 public:
  BlockStmtAST(std::vector<std::unique_ptr<StmtAST>> stmtList)
      : stmtList(std::move(stmtList)) {}
  virtual void print(int indentation) const override;
  virtual std::string getTypeString() const override { return "BlockStmt"; }
};

class ReturnStmtAST : public StmtAST {
  std::unique_ptr<ExprAST> returnExpr;

 public:
  ReturnStmtAST(std::unique_ptr<ExprAST> returnExpr)
      : returnExpr(std::move(returnExpr)) {}
  virtual void print(int indentation) const override;
  virtual std::string getTypeString() const override { return "ReturnStmt"; }
};

#endif  // STMT_AST_H

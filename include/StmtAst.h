// Statement Abstract Syntax Trees
#ifndef STMT_AST_H
#define STMT_AST_H
#include "Ast.h"

class ExprStmtAST : public StmtAST {
  std::unique_ptr<ExprAST> expr;
  std::shared_ptr<Program> programRoot;

 public:
  ExprStmtAST(std::shared_ptr<Program> programRoot,
              std::unique_ptr<ExprAST> expr)
      : programRoot(programRoot), expr(std::move(expr)) {}
  std::unique_ptr<ExprAST> &getExprRef() { return expr; }
  void print(int indentation) const override;
  std::string getTypeString() const override { return "ExprStmt"; }
  llvm::Value *codeGen() override;
};

class BlockStmtAST : public StmtAST {
  std::vector<std::unique_ptr<StmtAST>> stmtList;
  std::shared_ptr<Program> programRoot;

 public:
  BlockStmtAST(std::shared_ptr<Program> programRoot,
               std::vector<std::unique_ptr<StmtAST>> stmtList)
      : programRoot(programRoot), stmtList(std::move(stmtList)) {}
  void print(int indentation) const override;
  std::string getTypeString() const override { return "BlockStmt"; }
  llvm::Value *codeGen() override;
};

class ReturnStmtAST : public StmtAST {
  std::unique_ptr<ExprAST> returnExpr;
  std::shared_ptr<Program> programRoot;

 public:
  ReturnStmtAST(std::shared_ptr<Program> programRoot,
                std::unique_ptr<ExprAST> returnExpr)
      : programRoot(programRoot), returnExpr(std::move(returnExpr)) {}
  void print(int indentation) const override;
  std::string getTypeString() const override { return "ReturnStmt"; }
  llvm::Value *codeGen() override;
};

#endif  // STMT_AST_H

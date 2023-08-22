// Statement Codegen
#include "StmtAst.h"

llvm::Value *ExprStmtAST::codeGen() {}

llvm::Value *BlockStmtAST::codeGen() {
  for (const auto &stmt : stmtList) {
    stmt->codeGen();
  }
  return nullptr;
}

llvm::Value *ReturnStmtAST::codeGen() {
  return programRoot->getBuilder().CreateRet(returnExpr->codeGen());
};

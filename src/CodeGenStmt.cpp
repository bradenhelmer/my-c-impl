// Statement Codegen
#include "StmtAst.h"

llvm::Value *ExprStmtAST::codeGen() { return expr->codeGen(); }

llvm::Value *BlockStmtAST::codeGen() {
  for (const auto &stmt : stmtList) {
    stmt->codeGen();
  }
  return nullptr;
}

llvm::Value *ReturnStmtAST::codeGen() {
  if (returnExpr) {
    llvm::Value *returnVal = returnExpr->codeGen();
    if (llvm::AllocaInst *possibleAlloc =
            llvm::dyn_cast<llvm::AllocaInst>(returnVal)) {
      returnVal = programRoot->getBuilder().CreateLoad(
          possibleAlloc->getAllocatedType(), possibleAlloc);
    }
    return programRoot->getBuilder().CreateRet(returnVal);
  }
  return programRoot->getBuilder().CreateRetVoid();
};

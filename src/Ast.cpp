// Base AST implementations
#include "Ast.h"

#include <llvm/IR/Function.h>

Program::Program() {
  ctx = std::make_unique<llvm::LLVMContext>();
  module = std::make_unique<llvm::Module>("main", getContext());
  builder = std::make_unique<llvm::IRBuilder<>>(getContext());
}

llvm::Value *Program::codeGen() {
  for (const auto &decl : declList) {
    decl->codeGen();
  }
  return nullptr;
}

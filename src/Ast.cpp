// Base AST implementations
#include "Ast.h"

Program::Program() {
  ctx = std::make_unique<llvm::LLVMContext>();
  builder = std::make_unique<llvm::IRBuilder<>>(getContext());
  module = std::make_unique<llvm::Module>("main", getContext());
  symbolTable = std::make_unique<std::map<std::string, llvm::Value *>>();
}

llvm::Value *Program::codeGen() {
  for (const auto &decl : declList) {
    decl->codeGen();
  }
  return nullptr;
}

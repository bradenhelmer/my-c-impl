// Base AST implementations
#include "Ast.h"

#include <llvm/IR/Function.h>

Program::Program() {
  ctx = std::make_unique<llvm::LLVMContext>();
  module = std::make_unique<llvm::Module>("main", getContext());
  initMainFunction();
  builder = std::make_unique<llvm::IRBuilder<>>(getContext());
  entry = llvm::BasicBlock::Create(getContext(), "entry", main);
  setGlobalInsertion();
}

llvm::Value *Program::codeGen() {
  for (const auto &decl : declList) {
    decl->codeGen();
  }
  return nullptr;
}

void Program::initMainFunction() {
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getVoidTy(getContext()), false);
  main = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main",
                                getModule());
}

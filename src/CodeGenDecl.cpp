// Declaration code generation
#include <llvm/IR/Verifier.h>

#include <sstream>

#include "DeclAst.h"
#include "Diagnostics.h"
#include "llvm.h"

void VarDeclAST::checkReDeclaration() {
  Scope curr = programRoot->getCurrScope();
  std::stringstream stream;
  switch (curr) {
    case GLOBAL:
      if (programRoot->getModule().getGlobalVariable(id.idStr)) {
	stream << "Cannot redeclare global variable: " << id.idStr << " !";
	Diagnostic::runDiagnostic(Diagnostic::initialization_error,
	                          stream.str());
      }
      break;
    case FUNC:
      if (programRoot->getFuncVals()[id.idStr]) {
	stream << "Cannot redeclare function scoped variable: " << id.idStr
	       << " !";
	Diagnostic::runDiagnostic(Diagnostic::initialization_error,
	                          stream.str());
      }
      break;
    case COND:
      if (programRoot->getCondVals()[id.idStr]) {
	stream << "Cannot redeclare conditionally scoped variable: " << id.idStr
	       << " !";
	Diagnostic::runDiagnostic(Diagnostic::initialization_error,
	                          stream.str());
      }
      break;
  }
}

llvm::Value *VarDeclAST::codeGen() {
  checkReDeclaration();
  if (programRoot->getCurrScope() == GLOBAL) return createGlobalVariable();
  return allocVarDecl();
}

llvm::AllocaInst *VarDeclAST::allocVarDecl() {
  llvm::AllocaInst *allocation =
      programRoot->getBuilder().CreateAlloca(llvmType);
  if (expr) {
    llvm::Value *exprVal = expr->codeGen();
    if (exprVal->getType() != llvmType)
      Diagnostic::runDiagnostic(
          Diagnostic::initialization_error,
          "Varaible type does match that of expression in declaration!");
    programRoot->getBuilder().CreateStore(exprVal, allocation);
  }
  programRoot->storeValueToSymbolMap(allocation, id.idStr);
  return allocation;
}

llvm::GlobalVariable *VarDeclAST::createGlobalVariable(const bool isConstant) {
  llvm::GlobalVariable *GV = new llvm::GlobalVariable(
      programRoot->getModule(), llvmType, isConstant,
      llvm::GlobalVariable::LinkageTypes::ExternalLinkage, nullptr, id.idStr);
  if (expr) {
    llvm::Value *V = expr->codeGen();
    if (V->getType() != llvmType)
      Diagnostic::runDiagnostic(
          Diagnostic::initialization_error,
          "Initializer expression type does not match declaration type!");
    GV->setInitializer(llvm::dyn_cast<llvm::Constant>(V));
  }
  return GV;
}

llvm::Value *ConstVarDeclAST::codeGen() {
  checkReDeclaration();
  if (programRoot->getCurrScope() == GLOBAL) return createGlobalVariable(true);
  return createConstant();
}

llvm::Constant *ConstVarDeclAST::createConstant() {
  llvm::Value *V = expr->codeGen();
  if (V->getType() != llvmType)
    Diagnostic::runDiagnostic(
        Diagnostic::initialization_error,
        "Initializer expression type does not match declaration type!");
  programRoot->storeValueToSymbolMap(std::move(V), id.idStr);
  return llvm::cast<llvm::Constant>(V);
}

llvm::Function *PrototypeAST::codeGen() {
  std::vector<llvm::Type *> argTypes;
  for (const auto &arg : args) {
    argTypes.push_back(programRoot->getLLVMTypeFromToken(arg.type));
  }

  llvm::FunctionType *FT = llvm::FunctionType::get(llvmType, argTypes, false);

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, id.idStr, programRoot->getModule());
  unsigned idx = 0;
  for (auto &arg : F->args()) arg.setName(args[idx++].id.idStr);
  return F;
}

llvm::AllocaInst *FuncDeclAST::createEntryBlockAlloca(
    llvm::Function *func, llvm::Type *argType, const std::string &varName) {
  llvm::IRBuilder<> tempBuilder(&func->getEntryBlock(),
                                func->getEntryBlock().begin());
  return tempBuilder.CreateAlloca(argType, nullptr, varName);
}

llvm::Function *FuncDeclAST::codeGen() {
  // Tell the root node we are in function scope
  programRoot->setFuncScope();
  programRoot->setCurrFuncMapValPtr(&localVars);

  llvm::Function *func = programRoot->getModule().getFunction(proto->getName());

  if (!func) func = proto->codeGen();
  if (!func) return nullptr;
  if (!func->empty()) return LogErrorF("Cannot redefine function!");

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(programRoot->getContext(),
                                                  proto->getName(), func);
  programRoot->getBuilder().SetInsertPoint(BB);

  // Make arguments available to function scope
  localVars.clear();
  for (auto &arg : func->args()) {
    llvm::AllocaInst *argAlloc =
        createEntryBlockAlloca(func, arg.getType(), std::string(arg.getName()));
    programRoot->getBuilder().CreateStore(&arg, argAlloc);
    localVars[std::string(arg.getName())] = argAlloc;
  }

  body->codeGen();
  // Handle terminator if the function type is void
  if (func->getReturnType()->isVoidTy())
    programRoot->getBuilder().CreateRetVoid();

  if (!llvm::verifyFunction(*func, &llvm::errs())) {
    /* // Run basic function optimizations */
    programRoot->getFuncPassManager().run(
        *func, programRoot->getFunctionAnalysisManager());

    // Unset global scope notifier, func val ptr,  and clear insertion point
    programRoot->setGlobalScope();
    programRoot->setCurrFuncMapValPtr(nullptr);
    programRoot->getBuilder().ClearInsertionPoint();
    return func;
  }

  func->eraseFromParent();
  return nullptr;
}

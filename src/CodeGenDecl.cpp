// Declaration code generation
#include <llvm/IR/Verifier.h>

#include "DeclAst.h"
#include "llvm.h"

llvm::Value *VarDeclAST::codeGen() {
  Scope curr = programRoot->getCurrScope();
  switch (curr) {
    case GLOBAL:
      if (programRoot->getGlobals()[id.idStr])
	return LogErrorV("Cannot redeclare global variable!");
      break;
    case FUNC:
      if (programRoot->getFuncVals()[id.idStr])
	return LogErrorV("Cannot redeclare function scoped variable!");
      break;
    case COND:
      if (programRoot->getCondVals()[id.idStr])
	return LogErrorV("Cannot redeclare condition variable!");
      break;
  }
  llvm::Type *expectedType = getIntType(programRoot->getContext(), type);
  llvm::Value *initial = llvm::UndefValue::get(expectedType);
  if (expr) {
    llvm::Value *exprVal = expr->codeGen();
    if (exprVal->getType() != expectedType)
      return LogErrorV(
          "Expression does not evaluate to type declared for variable!");
    initial = exprVal;
  }
  switch (curr) {
    case GLOBAL: {
      llvm::GlobalVariable *global = new llvm::GlobalVariable(
          programRoot->getModule(), initial->getType(), false,
          llvm::GlobalVariable::ExternalLinkage,
          llvm::dyn_cast<llvm::Constant>(initial), id.idStr);
      programRoot->getGlobals()[id.idStr] = global;
      break;
    }
    case FUNC:
      programRoot->getFuncVals()[id.idStr] = initial;
      break;
    case COND:
      programRoot->getCondVals()[id.idStr] = initial;
      break;
  }
  return initial;
}

llvm::Function *PrototypeAST::codeGen() {
  std::vector<llvm::Type *> argTypes;
  for (const auto &arg : args) {
    argTypes.push_back(getIntType(programRoot->getContext(), arg.type));
  }

  llvm::FunctionType *FT = llvm::FunctionType::get(
      type == kw_void ? getVoidType(programRoot->getContext())
                      : getIntType(programRoot->getContext(), type),
      argTypes, false);

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, id.idStr, programRoot->getModule());
  unsigned idx = 0;
  for (auto &arg : F->args()) arg.setName(args[idx++].id.idStr);
  return F;
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
    localVars[std::string(arg.getName())] = &arg;
  }

  body->codeGen();
  if (!llvm::verifyFunction(*func, &llvm::errs())) {
    // Run basic function optimizations
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

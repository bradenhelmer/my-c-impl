// Declaration code generation
#include "DeclAst.h"
#include "llvm.h"

llvm::Value *VarDeclAST::codeGen() {
  if (programRoot->getGlobals()[id.idStr])
    return LogErrorV("Cannot re-declare variable!");
  llvm::Type *expectedType = getIntType(programRoot->getContext(), type);
  llvm::Value *initial = llvm::UndefValue::get(expectedType);
  if (expr) {
    llvm::Value *exprVal = expr->codeGen();
    if (exprVal->getType() != expectedType)
      return LogErrorV(
          "Expression does not evaluate to type declared for variable!");
    initial = exprVal;
  }
  /* programRoot->getGlobals()[id.idStr] = initial; */
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

llvm::Function *FuncDeclAST::codeGen() {}

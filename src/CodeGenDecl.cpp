// Declaration code generation
#include "DeclAst.h"
#include "llvm.h"

llvm::Value *VarDeclAST::codeGen() {
  if (programRoot->getSymbolTable()[id.idStr])
    return LogErrorV("Cannot re-declare variable!");
  llvm::Type *expectedType =
      getTypeFromPrimitive(programRoot->getContext(), type);
  llvm::Value *initial = llvm::UndefValue::get(expectedType);
  if (expr) {
    llvm::Value *exprVal = expr->codeGen();
    if (exprVal->getType() != expectedType)
      return LogErrorV(
          "Expression does not evaluate to type declared for variable!");
    initial = exprVal;
  }
  programRoot->getSymbolTable()[id.idStr] = initial;
  return initial;
}
llvm::Value *FuncDeclAST::codeGen() {}
llvm::Value *PrototypeAST::codeGen() {}

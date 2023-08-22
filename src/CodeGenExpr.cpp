// Expression code generation
#include "ExprAst.h"
#include "llvm.h"

llvm::Value *NumConstAST::codeGen() {
  // The primitive integer type for now will be 32 bits.
  return llvm::ConstantInt::get(
      llvm::Type::getInt32Ty(programRoot->getContext()), numConst);
}

llvm::Value *CharConstAST::codeGen() {
  return llvm::ConstantInt::get(programRoot->getContext(),
                                llvm::APInt(CHAR_BIT_LENGTH, charConst));
}

llvm::Value *StringLiteralAST::codeGen() {}

llvm::Value *BinaryExprAST::codeGen() {
  llvm::Value *leftVal = LHS->codeGen();
  llvm::Value *rightVal = RHS->codeGen();

  if (!leftVal || !rightVal) return LogErrorV("Could not evaluate expression!");

  switch (op) {
    case plus:
      return programRoot->getBuilder().CreateAdd(leftVal, rightVal, "addtmp");
    case minus:
      return programRoot->getBuilder().CreateSub(leftVal, rightVal, "subtmp");
    case star:
      return programRoot->getBuilder().CreateMul(leftVal, rightVal, "multmp");
    case slash:
      return programRoot->getBuilder().CreateUDiv(leftVal, rightVal, "divtmp");
    case percent:
      return programRoot->getBuilder().CreateURem(leftVal, rightVal, "modtmp");
    case less:
      return programRoot->getBuilder().CreateICmpULT(leftVal, rightVal,
                                                     "booltmpl");
    case less_equal:
      return programRoot->getBuilder().CreateICmpULE(leftVal, rightVal,
                                                     "booltmple");
    case greater:
      return programRoot->getBuilder().CreateICmpUGT(leftVal, rightVal,
                                                     "booltmpg");
    case greater_equal:
      return programRoot->getBuilder().CreateICmpUGE(leftVal, rightVal,
                                                     "booltmpge");
    default:
      return LogErrorV("Invalid binary operator!");
  }
}

llvm::Value *VarExprAST::codeGen() {
  llvm::Value *varVal;
  switch (programRoot->getCurrScope()) {
    case GLOBAL: {
      if ((varVal = programRoot->getGlobals()[name])) {
	return loadGlobal(varVal, programRoot->getBuilder());
      } else {
	return LogErrorV("Variable not found!");
      }
    }
    case FUNC: {
      if ((varVal = programRoot->getFuncVals()[name])) {
	break;
      } else if ((varVal = programRoot->getGlobals()[name])) {
	return loadGlobal(varVal, programRoot->getBuilder());
      } else {
	return LogErrorV("Variable not found!");
      }
    }
    case COND: {
      if ((varVal = programRoot->getCondVals()[name])) {
	break;
      } else if ((varVal = programRoot->getFuncVals()[name])) {
	break;
      } else if ((varVal = programRoot->getGlobals()[name])) {
	return loadGlobal(varVal, programRoot->getBuilder());
      } else {
	return LogErrorV("Variable not found!");
      }
    }
    default:
      return LogErrorV("Error retrieving variable");
  }
}
llvm::Value *CallExprAST::codeGen() {
  llvm::Function *calleeF = programRoot->getModule().getFunction(callee);
  if (!calleeF) return LogErrorV("Unknown function referenced");
  if (calleeF->arg_size() != args.size())
    return LogErrorV("Incorrect # of arguments passed!");
  std::vector<llvm::Value *> argVals;
  for (unsigned i = 0, e = args.size(); i != e; ++i) {
    argVals.push_back(args[i]->codeGen());
    if (!argVals.back()) return nullptr;
  }

  return programRoot->getBuilder().CreateCall(calleeF, argVals, "calltmp");
}

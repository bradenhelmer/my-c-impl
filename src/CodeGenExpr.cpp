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

  /* if (isAssignmentOperator(op)) return codeGenAssignment(leftVal, rightVal,
   * op); */

  switch (op) {
    case equal:
      return programRoot->getBuilder().CreateStore(rightVal, leftVal);
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

llvm::Value *BinaryExprAST::codeGenAssignment(llvm::Value *RHS,
                                              llvm::Value *LHS, TokenKind op) {
  switch (op) {
    case equal:
      return programRoot->getBuilder().CreateStore(LHS, RHS);
    case plus_equal:
      break;
    case minus_equal:
      break;
    case star_equal:
      break;
    case slash_equal:
      break;
    default:
      break;
  }
  return nullptr;
}

llvm::Value *VarExprAST::codeGen() {
  llvm::AllocaInst *possibleAlloc = nullptr;
  llvm::GlobalVariable *GV = nullptr;

  switch (programRoot->getCurrScope()) {
    case GLOBAL: {
      if ((GV = programRoot->getGlobals()[name])) {
	break;
      } else {
	return LogErrorV("Variable not found!");
      }
    }
    case FUNC: {
      if ((possibleAlloc = programRoot->getFuncVals()[name])) {
	break;
      } else if ((GV = programRoot->getGlobals()[name])) {
	break;
      } else {
	return LogErrorV("Variable not found!");
      }
    }
    case COND: {
      if ((possibleAlloc = programRoot->getCondVals()[name])) {
	break;
      } else if ((possibleAlloc = programRoot->getFuncVals()[name])) {
	break;
      } else if ((GV = programRoot->getGlobals()[name])) {
	break;
      } else {
	return LogErrorV("Variable not found!");
      }
    }
    default:
      return LogErrorV("Error retrieving variable");
  }
  if (GV) return programRoot->getBuilder().CreateLoad(GV->getValueType(), GV);
  return programRoot->getBuilder().CreateLoad(possibleAlloc->getAllocatedType(),
                                              possibleAlloc, name);
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
  return calleeF->getReturnType()->isVoidTy()
             ? programRoot->getBuilder().CreateCall(calleeF, argVals)
             : programRoot->getBuilder().CreateCall(calleeF, argVals,
                                                    "calltmp");
}

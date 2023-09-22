// Expression code generation
#include "Diagnostics.h"
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

llvm::Value *StringLiteralAST::codeGen() {
  return llvm::UndefValue::get(llvm::ArrayType::get(
      llvm::Type::getInt8Ty(programRoot->getContext()), 0));
}

llvm::Value *BinaryExprAST::codeGen() {
  if (isAssignmentOperator(op)) return codeGenAssignment();

  llvm::Value *leftVal = LHS->codeGen();
  llvm::Value *rightVal = RHS->codeGen();

  if (!leftVal || !rightVal)
    Diagnostic::runDiagnostic(Diagnostic::type_error,
                              "Could not evaluate expression!");

  if (llvm::AllocaInst *possibleAlloc =
          llvm::dyn_cast<llvm::AllocaInst>(rightVal)) {
    rightVal = programRoot->getBuilder().CreateLoad(
        possibleAlloc->getAllocatedType(), possibleAlloc,
        possibleAlloc->getName());
  }

  if (llvm::AllocaInst *possibleAlloc =
          llvm::dyn_cast<llvm::AllocaInst>(leftVal)) {
    leftVal = programRoot->getBuilder().CreateLoad(
        possibleAlloc->getAllocatedType(), possibleAlloc,
        possibleAlloc->getName());
  }

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
      Diagnostic::runDiagnostic(Diagnostic::token_error,
                                "Invalid binary operator!");
  }
  return nullptr;
}

llvm::Value *BinaryExprAST::codeGenAssignment() {
  llvm::Value *leftVal = LHS->codeGen();
  llvm::Value *rightVal = RHS->codeGen();

  if (!leftVal || !rightVal)
    Diagnostic::runDiagnostic(Diagnostic::type_error,
                              "Could not evaluate expression!");

  if (llvm::isa<llvm::Constant>(leftVal)) {
    Diagnostic::runDiagnostic(Diagnostic::initialization_error,
                              "Cannot re-assign variable declared const!");
  }
  switch (op) {
    case equal:
      return programRoot->getBuilder().CreateStore(rightVal, leftVal);
    case plus_equal:
      return programRoot->getBuilder().CreateStore(
          programRoot->getBuilder().CreateAdd(leftVal, rightVal), leftVal);
    case minus_equal:
      return programRoot->getBuilder().CreateStore(
          programRoot->getBuilder().CreateSub(leftVal, rightVal), leftVal);
    case star_equal:
      return programRoot->getBuilder().CreateStore(
          programRoot->getBuilder().CreateMul(leftVal, rightVal), leftVal);
    case slash_equal:
      return programRoot->getBuilder().CreateStore(
          programRoot->getBuilder().CreateUDiv(leftVal, rightVal), leftVal);
    default:
      break;
  }
  return nullptr;
}

llvm::Value *VarExprAST::codeGen() {
  llvm::Value *val;
  llvm::GlobalVariable *GV = nullptr;

  switch (programRoot->getCurrScope()) {
    case GLOBAL: {
      if ((GV = programRoot->getModule().getGlobalVariable(name))) {
	break;
      } else {
	Diagnostic::runDiagnostic(Diagnostic::unknwon_symbol_error,
	                          "Error retrieving global variable!");
      }
    }
    case FUNC: {
      if ((val = programRoot->getFuncVals()[name])) {
	break;
      } else if ((GV = programRoot->getModule().getGlobalVariable(name))) {
	break;
      } else {
	Diagnostic::runDiagnostic(
	    Diagnostic::unknwon_symbol_error,
	    "Error retrieving variable either function or global scoped!");
      }
    }
    case COND: {
      if ((val = programRoot->getCondVals()[name])) {
	break;
	break;
      } else if ((val = programRoot->getFuncVals()[name])) {
      } else if ((GV = programRoot->getModule().getGlobalVariable(name))) {
	break;
      } else {
	Diagnostic::runDiagnostic(Diagnostic::unknwon_symbol_error,
	                          "Error retrieving variable conditionally, "
	                          "function or global scoped!");
      }
    }
    default:
      Diagnostic::runDiagnostic(Diagnostic::unknwon_symbol_error,
                                "Error retrieving variable");
  }
  if (GV) return programRoot->getBuilder().CreateLoad(GV->getValueType(), GV);
  return val;
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

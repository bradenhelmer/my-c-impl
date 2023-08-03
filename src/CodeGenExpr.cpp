// Expression code generation
#include "ExprAst.h"
#include "llvm.h"

llvm::Value *NumConstAST::codeGen() {
  // The primitive integer type for now will be 32 bits.
  return llvm::ConstantInt::get(programRoot->getContext(),
                                llvm::APInt(numConst, INT_BIT_LENGTH));
}

llvm::Value *CharConstAST::codeGen() {
  return llvm::ConstantInt::get(programRoot->getContext(),
                                llvm::APInt(CHAR_BIT_LENGTH, charConst));
}

llvm::Value *StringLiteralAST::codeGen() {}
llvm::Value *BinaryExprAST::codeGen() {}
llvm::Value *VarExprAST::codeGen() {}
llvm::Value *CallExprAST::codeGen() {}

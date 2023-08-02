// Expression code generation
#include "ExprAst.h"

llvm::Value *NumConstAST::codeGen() {}
llvm::Value *CharConstAST::codeGen() {}
llvm::Value *StringLiteralAST::codeGen() {}
llvm::Value *BinaryExprAST::codeGen() {}
llvm::Value *VarExprAST::codeGen() {}
llvm::Value *CallExprAST::codeGen() {}

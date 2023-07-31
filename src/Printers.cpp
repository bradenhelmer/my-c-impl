// Printers.cpp
// ~~~~~~~~~~~~
// Implementation of AST printer funtions
#include "Ast.h"
#include "Common.h"
#include "DeclAst.h"
#include "ExprAst.h"
#include "StmtAst.h"

void Program::print(int indentation) const {
  LLVM_OUT_NL("Printing Abstract Syntax Tree");
  LLVM_OUT_NL("-----------------------------");
  LLVM_OUT_NL(getTypeString());
  for (const auto& decl : declList) {
    decl->print(indentation + 1);
  }
}

void VarDeclAST::print(int indentation) const {
  std::string indent(indentation, INDENT_CHAR);
  LLVM_OUT(indent);
  std::string varDeclStr = getTypeString() + ": ";
  LLVM_OUT_NL(varDeclStr + constructVarDeclString());
  if (expr != NULL) {
    expr->print(indentation + 1);
  }
}

void FuncDeclAST::print(int indentation) const {
  std::string indent(indentation, INDENT_CHAR);
  LLVM_OUT(indent);
  std::string protoStr = getTypeString() + ": ";
  LLVM_OUT_NL(protoStr + proto->constructProtoString());
  if (body != NULL) {
    body->print(indentation + 1);
  }
}
void ExprAST::print(int indenation) const {}
void NumConstAST::print(int indentation) const {
  std::string indent(indentation, INDENT_CHAR);
  LLVM_OUT(indent);
  LLVM_OUT_NL(getTypeString() + ": " + std::to_string(numConst));
}

void CallExprAST::print(int indentation) const {
  std::string indent(indentation, INDENT_CHAR);
  LLVM_OUT(indent);
  LLVM_OUT_NL(constructCallStr());
}

void ExprStmtAST::print(int indentation) const {
  std::string indent(indentation, INDENT_CHAR);
  LLVM_OUT(indent);
  LLVM_OUT_NL(getTypeString());
  expr->print(indentation + 1);
}

void BlockStmtAST::print(int indentation) const {
  for (const auto& stmt : stmtList) stmt->print(indentation);
}

void ReturnStmtAST::print(int indentation) const {
  std::string indent(indentation, INDENT_CHAR);
  LLVM_OUT(indent);
  LLVM_OUT(getTypeString() + ": ");
  if (returnExpr != NULL) {
    returnExpr->print(indentation + 1);
  } else {
    LLVM_OUT_NL("None");
  }
}

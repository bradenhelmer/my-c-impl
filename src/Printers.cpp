// Printers.cpp
// ~~~~~~~~~~~~
// Implementation of AST printer funtions
#include "Ast.h"
#include "Common.h"

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
void ExprAST::print(int indentation) const {}
void BlockStmtAST::print(int indentation) const {}

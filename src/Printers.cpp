// Printers.cpp
// ~~~~~~~~~~~~
// Implementation of AST printer funtions
#include <llvm/Support/raw_ostream.h>

#include "Ast.h"

#define INDENT_CHAR '\t'
#define LLVM_OUT_NL(X) \
  llvm::outs() << X;   \
  llvm::outs() << '\n';

void Program::print(int indentation) {
  std::string ident(indentation, INDENT_CHAR);
  LLVM_OUT_NL("Program");
  for (const auto& decl : declList) {
    decl->print(indentation + 1);
  }
}

void DeclAST::print(int indentation) {
  std::string(indentation, INDENT_CHAR);
  return;
}

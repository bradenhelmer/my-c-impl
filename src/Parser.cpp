// Parser.cpp
// ~~~~~~~~~~
// Parser implementation
#include "Parser.h"

#include "Common.h"

std::unique_ptr<Program> Parser::parseProgram() {
  // Advance current token to begin parsing.
  advanceCurrent();

  // Create new LLVM objects for the AST
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);
  llvm::Module module("module", context);
  std::map<std::string, llvm::Value *> symbolTable;

  // Parse
  return std::make_unique<Program>(context, builder, module, symbolTable,
                                   parseDeclList());
}

void Parser::advanceCurrent() {
  lex.advanceToken();
  currTok = lex.getCurrentToken();
}

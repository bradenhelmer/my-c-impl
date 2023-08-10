// ast.h
// ~~~~~
// Basic Syntax Tree defintions
#ifndef AST_H
#define AST_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <memory>
#include <vector>

#include "Common.h"
#include "Token.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
  virtual void print(int indentation) const { LLVM_OUT_NL(getTypeString()); }
  virtual std::string getTypeString() const { return "AstNode"; }
  virtual llvm::Value *codeGen() = 0;

  static llvm::Value *LogErrorV(const char *str) {
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
  }
};

class DeclAST : public AstNode {
 public:
  llvm::Value *codeGen() override {}
};

class ExprAST : public AstNode {
 public:
  llvm::Value *codeGen() override {}
};

class StmtAST : public AstNode {
 public:
  virtual std::unique_ptr<ExprAST> &getExprRef() = 0;
  llvm::Value *codeGen() override {}
};

enum Scope : unsigned short { GLOBAL = 0, FUNC = 1, COND = 2 };

class Program : public AstNode, public std::enable_shared_from_this<Program> {
  // Top Level Declarations of the program
  std::vector<std::unique_ptr<DeclAST>> declList;

  // LLVM Things
  std::unique_ptr<llvm::LLVMContext> ctx;
  std::unique_ptr<llvm::Module> module;
  llvm::Function *main;
  llvm::BasicBlock *entry;
  std::unique_ptr<llvm::IRBuilder<>> builder;

  // Current scope
  Scope currGenScope = GLOBAL;

  // Global scoped variables and coressponding vals.
  std::map<std::string, llvm::GlobalVariable *> globals;

  // Pointer to current function scoped values being generated.
  std::map<std::string, llvm::Value *> *currFuncVals = nullptr;

  // Pointer to current condition scoped values being generated.
  std::map<std::string, llvm::Value *> *currCondVals = nullptr;

  // Initializes the main (global) function for the program root.
  void initMainFunction();

 public:
  Program();
  void print(int indentation = 0) const override;
  std::string getTypeString() const override { return "Program"; }
  llvm::Value *codeGen() override;
  void attachDecls(std::vector<std::unique_ptr<DeclAST>> decls) {
    declList = std::move(decls);
  }

  llvm::LLVMContext &getContext() const { return *ctx; }

  llvm::IRBuilder<> &getBuilder() const { return *builder; }

  llvm::Module &getModule() const { return *module; }

  Scope getCurrScope() const { return currGenScope; }

  void setGlobalScope() { currGenScope = GLOBAL; }

  void setFuncScope() { currGenScope = FUNC; }

  void setCondScope() { currGenScope = COND; }

  std::map<std::string, llvm::GlobalVariable *> &getGlobals() {
    return globals;
  }

  std::map<std::string, llvm::Value *> &getFuncVals() { return *currFuncVals; }

  std::map<std::string, llvm::Value *> &getCondVals() { return *currCondVals; }

  void setCurrFuncMapValPtr(std::map<std::string, llvm::Value *> *ptr) {
    currFuncVals = ptr;
  }
  void setCurrCondMapValPtr(std::map<std::string, llvm::Value *> *ptr) {
    currCondVals = ptr;
  }

  void setGlobalInsertion() {
    builder->ClearInsertionPoint();
    builder->SetInsertPoint(entry);
  }
};

#endif  // AST_H

// ast.h
// ~~~~~
// Basic Syntax Tree defintions
#ifndef AST_H
#define AST_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Passes/PassBuilder.h>

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

  // LLVM Base members
  std::unique_ptr<llvm::LLVMContext> ctx;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;

  // LLVM Optimization members
  std::unique_ptr<llvm::FunctionPassManager> FPM;
  std::unique_ptr<llvm::ModulePassManager> MPM;
  std::unique_ptr<llvm::LoopAnalysisManager> LAM;
  std::unique_ptr<llvm::FunctionAnalysisManager> FAM;
  std::unique_ptr<llvm::CGSCCAnalysisManager> CGAM;
  std::unique_ptr<llvm::ModuleAnalysisManager> MAM;

  // Current scope
  Scope currGenScope = GLOBAL;

  // Global scoped variables and coressponding vals.
  std::map<std::string, llvm::GlobalVariable *> globals;

  // Pointer to current function scoped values being generated.
  std::map<std::string, llvm::Value *> *currFuncVals = nullptr;

  // Pointer to current condition scoped values being generated.
  std::map<std::string, llvm::Value *> *currCondVals = nullptr;

  // Initializes all analysis managers.
  void initAnalysisManagers();
  // Initializes module and function pass managers.
  void initPassManagers();

  // Runs the default optimization pipeline for the module.
  void runDefaultOptimization() const {
    getModulePassManager().run(getModule(), getModuleAnalysisManager());
  }

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

  llvm::FunctionPassManager &getFuncPassManager() const { return *FPM; }

  llvm::ModulePassManager &getModulePassManager() const { return *MPM; }

  llvm::LoopAnalysisManager &getLoopAnalysisManager() const { return *LAM; }

  llvm::FunctionAnalysisManager &getFunctionAnalysisManager() const {
    return *FAM;
  }

  llvm::CGSCCAnalysisManager &getCGSCCAnalysisManager() const { return *CGAM; }

  llvm::ModuleAnalysisManager &getModuleAnalysisManager() const { return *MAM; }

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
};

#endif  // AST_H

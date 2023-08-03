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
};

class DeclAST : public AstNode {};

class ExprAST : public AstNode {};

class StmtAST : public AstNode {
 public:
  virtual std::unique_ptr<ExprAST> &getExprRef() {}
};

class Program : public AstNode, public std::enable_shared_from_this<Program> {
  std::vector<std::unique_ptr<DeclAST>> declList;
  std::unique_ptr<llvm::LLVMContext> ctx;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<std::map<std::string, llvm::Value *>> symbolTable;

 public:
  Program();
  void print(int indentation = 0) const override;
  std::string getTypeString() const override { return "Program"; }
  virtual llvm::Value *codeGen() override;
  void attachDecls(std::vector<std::unique_ptr<DeclAST>> decls) {
    declList = std::move(decls);
  }

  llvm::LLVMContext &getContext() const { return *ctx; }

  llvm::IRBuilder<> &getBuilder() const { return *builder; }

  llvm::Module &getModule() const { return *module; }

  std::map<std::string, llvm::Value *> &getSymbolTable() const {
    return *symbolTable;
  }
};

#endif  // AST_H

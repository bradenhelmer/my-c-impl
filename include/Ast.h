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

 protected:
  static llvm::LLVMContext &context;
  static llvm::IRBuilder<> &builder;
  static llvm::Module &module;
  static std::map<std::string, llvm::Value *> &symbolTable;
};

class DeclAST : public AstNode {};

class ExprAST : public AstNode {};

class StmtAST : public AstNode {
 public:
  virtual std::unique_ptr<ExprAST> &getExprRef() {}
};

class Program : public AstNode {
  std::vector<std::unique_ptr<DeclAST>> declList;

 public:
  Program(std::vector<std::unique_ptr<DeclAST>> declList)
      : declList(std::move(declList)) {}
  void print(int indentation = 0) const override;
  std::string getTypeString() const override { return "Program"; }
  virtual llvm::Value *codeGen() override;
};

#endif  // AST_H

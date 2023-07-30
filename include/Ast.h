// ast.h
// ~~~~~
// Basic Syntax Tree defintions
#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>

#include "Common.h"
#include "Token.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
  virtual void print(int indentation) const { LLVM_OUT_NL(getTypeString()); }
  virtual std::string getTypeString() const { return "AstNode"; }
};

class DeclAST : public AstNode {};

class ExprAST : public AstNode {
 public:
  virtual void print(int indentation) const override;
};

class StmtAST : public AstNode {};

class Program : public AstNode {
  std::vector<std::unique_ptr<DeclAST>> declList;

 public:
  Program(std::vector<std::unique_ptr<DeclAST>> declList)
      : declList(std::move(declList)) {}
  virtual void print(int indentation = 0) const override;
  virtual std::string getTypeString() const override { return "Program"; }
};

#endif  // AST_H

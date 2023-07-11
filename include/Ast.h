// ast.h
// ~~~~~
// Abstract Syntax Tree defintions
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "Token.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
};

class DeclAST : public AstNode {};

class ExprAST : public AstNode {};

class StmtAST : public AstNode {};

class Program : public AstNode {
  std::vector<std::unique_ptr<DeclAST>> declList;

 public:
  Program(std::vector<std::unique_ptr<DeclAST>> declList)
      : declList(std::move(declList)) {}
};

class VarDeclAST : public DeclAST {
  TokenKind type;
  Identifier id;
  std::unique_ptr<ExprAST> expr;

 public:
  VarDeclAST(TokenKind type, Identifier id, std::unique_ptr<ExprAST> expr)
      : id(std::move(id)), type(type), expr(std::move(expr)) {}
};

class PrototypeAST : public DeclAST {
  TokenKind type;
  Identifier id;
  std::vector<FuncParam> args;

 public:
  PrototypeAST(TokenKind type, Identifier id, std::vector<FuncParam> args)
      : type(type), id(std::move(id)), args(std::move(args)) {}
};

class BlockStmtAST;
class FuncDeclAST : public DeclAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<BlockStmtAST> body;

 public:
  FuncDeclAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<BlockStmtAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}
};

class BlockStmtAST : public StmtAST {
  std::vector<std::unique_ptr<VarDeclAST>> localDecls;
  std::vector<std::unique_ptr<StmtAST>> stmtList;

 public:
  BlockStmtAST(std::vector<std::unique_ptr<VarDeclAST>> localDecls,
               std::vector<std::unique_ptr<StmtAST>> stmtList)
      : localDecls(std::move(localDecls)), stmtList(std::move(stmtList)) {}
};

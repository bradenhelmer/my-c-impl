// Declaration Abstract Syntax Tress
#ifndef DECL_AST_H
#define DECL_AST_H
#include "Ast.h"

class VarDeclAST : public DeclAST, public StmtAST {
  TokenKind type;
  Identifier id;
  std::unique_ptr<ExprAST> expr;

 public:
  VarDeclAST(TokenKind type, Identifier id, std::unique_ptr<ExprAST> expr)
      : id(std::move(id)), type(type), expr(std::move(expr)) {}
  virtual void print(int indentation) const override;
  virtual std::string getTypeString() const override { return "VarDecl"; }
  std::string getArrayId() const {
    std::string arrayId = id.idStr;
    arrayId += '[' + std::to_string(id.size) + ']';
    return arrayId;
  }
  std::string constructVarDeclString() const {
    std::string varDeclStr = '\'' + (id.isArray ? getArrayId() : id.idStr) +
                             '\'' + ": " + getPrimitiveName(type);
    return varDeclStr;
  }
};

class PrototypeAST : public DeclAST {
  TokenKind type;
  Identifier id;
  std::vector<FuncParam> args;

 public:
  PrototypeAST(TokenKind type, Identifier id, std::vector<FuncParam> args)
      : type(type), id(std::move(id)), args(std::move(args)) {}
  std::string constructProtoString() const {
    std::string protoStr =
        '\'' + id.idStr + '\'' + " -> " + getPrimitiveName(type);
    return protoStr;
  }
};

class BlockStmtAST;
class FuncDeclAST : public DeclAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<BlockStmtAST> body;

 public:
  FuncDeclAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<BlockStmtAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}
  virtual void print(int indentation) const override;
  virtual std::string getTypeString() const override { return "FuncDecl"; }
};

#endif  // DECL_AST_H

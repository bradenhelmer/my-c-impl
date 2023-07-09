// ast.h
// ~~~~~
// Abstract Syntax Tree defintions
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Token.h"

class AstNode {
public:
  virtual ~AstNode() = default;
};

class ExprAST : public AstNode {};

class DeclAST : public AstNode {};

class NumberExprAST : public ExprAST {
  double value;

public:
  NumberExprAST(double value) : value(value) {}
};

class VariableDeclInitAST : public DeclAST {
  TokenKind type;
  std::string name;
  std::unique_ptr<ExprAST> expr;

public:
  VariableDeclInitAST(TokenKind type, const std::string name,
                      std::unique_ptr<ExprAST> expr)
      : type(type), name(name), expr(std::move(expr)) {}
};

class VariableDeclAST : public DeclAST {
  TokenKind type;
  std::string name;

public:
  VariableDeclAST(TokenKind type, const std::string name)
      : type(type), name(name) {}
};

class VariableAST : public ExprAST {
  std::string name;

public:
  VariableAST(const std::string name) : name(name) {}
};

class BinaryExprAST : public ExprAST {
  TokenKind op;
  std::unique_ptr<ExprAST> left, right;

public:
  BinaryExprAST(TokenKind op, std::unique_ptr<ExprAST> left,
                std::unique_ptr<ExprAST> right)
      : op(op), left(std::move(left)), right(std::move(right)) {}
};

class CallExprAST : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
      : callee(callee), args(std::move(args)) {}
};

class PrototypeAST : public DeclAST {
  TokenKind type;
  std::string name;
  std::map<std::string, TokenKind> args;

public:
  PrototypeAST(TokenKind type, const std::string &name,
               std::map<std::string, TokenKind> args)
      : type(type), name(name), args(std::move(args)) {}
  const std::string &getName() const { return name; }
};

class FunctionAST : public DeclAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}
};

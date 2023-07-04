// ast.h
// ~~~~~
// Abstract Syntax Tree defintions
#include <map>
#include <memory>
#include <string>
#include <vector>

class AstNode {
 public:
  virtual ~AstNode() = default;
};

class StmtAST : public AstNode {};

class BlockAST : public AstNode {
  std::vector<std::unique_ptr<StmtAST>> stmts;

 public:
  BlockAST(std::vector<std::unique_ptr<StmtAST>> stmts)
      : stmts(std::move(stmts)) {}
};

class ExprAST : public AstNode {};

class ExprStmtAST : public StmtAST {
  std::unique_ptr<ExprAST> expr;

 public:
  ExprStmtAST(std::unique_ptr<ExprAST> expr) : expr(std::move(expr)) {}
};
class NumberExprAST : public ExprAST {
  double value;

 public:
  NumberExprAST(double value) : value(value) {}
};

class VariableExprAST : public ExprAST {
  std::string name, type;

 public:
  VariableExprAST(const std::string &name, const std::string &type)
      : name(name), type(type) {}
};

class BinaryExprAST : public ExprAST {
  char op;
  std::unique_ptr<ExprAST> left, right;

 public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> left,
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

class PrototypeAST {
  std::string name;
  std::map<std::string, std::string> args;

 public:
  PrototypeAST(const std::string &name,
               std::map<std::string, std::string> args)
      : name(name), args(std::move(args)) {}
  const std::string &getName() const { return name; }
};

class FunctionAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;

 public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}
};

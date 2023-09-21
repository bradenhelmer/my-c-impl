// Declaration Abstract Syntax Tress
#ifndef DECL_AST_H
#define DECL_AST_H
#include "Ast.h"
#include "Diagnostics.h"
#include "StmtAst.h"

class VarDeclAST : public DeclAST, public StmtAST {
 public:
  VarDeclAST(std::shared_ptr<Program> programRoot, TokenKind type,
             Identifier id, std::unique_ptr<ExprAST> expr)
      : programRoot(programRoot),
        id(std::move(id)),
        llvmType(programRoot->getLLVMTypeFromToken(type)),
        type(type),
        expr(std::move(expr)) {
    if (type == kw_void)
      Diagnostic::runDiagnostic(Diagnostic::initialization_error,
                                "Cannot initialize variable of type void!");
  }

  void print(int indentation) const override;
  std::unique_ptr<ExprAST> &getExprRef() { return expr; }
  std::string getTypeString() const override { return "VarDecl"; }
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
  llvm::Value *codeGen() override;
  llvm::GlobalVariable *createGlobalVariable(const bool isConstant = false);
  llvm::AllocaInst *allocVarDecl();

 protected:
  // Checks to see if the value being generated has already been declared.
  void checkReDeclaration();
  TokenKind type;
  llvm::Type *llvmType;
  Identifier id;
  std::unique_ptr<ExprAST> expr;
  std::shared_ptr<Program> programRoot;
};

class ConstVarDeclAST : public VarDeclAST {
 public:
  using VarDeclAST::VarDeclAST;

  std::string getTypeString() const override { return "ConstVarDecl"; }
  llvm::Value *codeGen() override;
  llvm::Constant *createConstant();
};

class PrototypeAST : public DeclAST {
  TokenKind type;
  llvm::Type *llvmType;
  Identifier id;
  std::vector<FuncParam> args;
  std::shared_ptr<Program> programRoot;

 public:
  PrototypeAST(std::shared_ptr<Program> programRoot, TokenKind type,
               Identifier id, std::vector<FuncParam> args)
      : programRoot(programRoot),
        type(type),
        llvmType(programRoot->getLLVMTypeFromToken(type)),
        id(std::move(id)),
        args(std::move(args)) {}
  std::string constructProtoString() const {
    std::string protoStr =
        '\'' + id.idStr + '\'' + " -> " + getPrimitiveName(type);
    return protoStr;
  }
  std::string getName() const { return id.idStr; }

  llvm::Function *codeGen();
};

class FuncDeclAST : public DeclAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<BlockStmtAST> body;
  std::map<std::string, llvm::Value *> localVars;
  std::shared_ptr<Program> programRoot;
  llvm::AllocaInst *createEntryBlockAlloca(llvm::Function *func,
                                           llvm::Type *argType,
                                           const std::string &varName);

 public:
  FuncDeclAST(std::shared_ptr<Program> programRoot,
              std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<BlockStmtAST> body)
      : programRoot(programRoot),
        proto(std::move(proto)),
        body(std::move(body)) {}
  void print(int indentation) const override;
  std::string getTypeString() const override { return "FuncDecl"; }
  llvm::Function *codeGen() override;

  static llvm::Function *LogErrorF(const char *str) {
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
  }
};

#endif  // DECL_AST_H

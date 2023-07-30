// Parser.h
// --------
// Parser definiton
#ifndef PARSER_H
#define PARSER_H
#include <memory>

#include "Ast.h"
#include "Lexer.h"
#include "Token.h"

class Parser {
  Lexer lex;
  Token *currTok;

 public:
  Parser(Lexer &lex) : lex(lex) {}

  // Parser entry point.
  // program -> declList
  std::unique_ptr<Program> parseProgram();

  // Node to log errors.
  template <typename T>
  static std::unique_ptr<T> LogError(const char *str);

 private:
  // Advance current token from lexer and set currTok
  void advanceCurrent();

  // Parse all declarations from source.
  // declList -> declList decl | decl
  std::vector<std::unique_ptr<DeclAST>> parseDeclList();

  // Parse a declaration, either a variable or function.
  // decl -> varDecl | funDecl
  std::unique_ptr<DeclAST> parseDecl();

  // Parse a function declaration, first constructing a
  // prototype then parsing throught the fucntion block.
  // funDecl -> primType IDENTIFIER(params) blockStmt
  std::unique_ptr<FuncDeclAST> parseFuncDecl(TokenKind kind,
                                             const Identifier &id);

  // Parse a function prototype
  // IDENTIFIER(params)
  //  params -> paramList | E
  //  paramList -> paramList, param | param
  //  param -> primType paramId
  //  paramId -> IDENTIFIER | IDENTIFIER[NUMCONST]
  std::unique_ptr<PrototypeAST> parseProtoType(TokenKind kind,
                                               const Identifier &id);

  // Parse a variable declaration with or without initialization.
  // varDecl -> primType varDeclInit
  // varDeclInit -> varDeclId | varDeclId = simpleExpr
  // varDeclId -> IDENTIFIER | IDENTIFIER[NUMCONST]
  std::unique_ptr<VarDeclAST> parseVarDecl(TokenKind kind,
                                           const Identifier &id);

  // Parse an expression that can be evaluated to some value
  // expr -> mutable = expr | mutable += expr |
  //  mutable -= expr | mutable *= expr |
  //  mutable /= expr | mutable++ | mutable-- | simpleExpr
  std::unique_ptr<ExprAST> parseExpr();

  // Parse a block statement
  // blockStmt -> { localDecls stmtList }
  std::unique_ptr<BlockStmtAST> parseBlockStmt();

  // Parse a condition statment
  // condStmt -> if (simpleExpr) blockStmt |
  // if (simpleExpr) blockStmt else blockStmt
  std::unique_ptr<BlockStmtAST> parseCondStatement();

  // Parse a for or a while statement
  // iterStmt -> while(simpleExpr) blockStmt |
  // for (simpleExpr; simpleExpr; simpleExpr) blockStmt
  std::unique_ptr<BlockStmtAST> parseIterStmt();

  // Parse a return statement
  // returnStmt -> return; | return expr;
  std::unique_ptr<BlockStmtAST> parseReturnStmt();
};

#endif  // PARSER_H

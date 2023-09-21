// Parser.h
// --------
// Parser definiton
#ifndef PARSER_H
#define PARSER_H
#include <memory>

#include "Ast.h"
#include "DeclAst.h"
#include "ExprAst.h"
#include "Lexer.h"
#include "StmtAst.h"
#include "Token.h"

class Parser {
  Lexer lex;
  Token *currTok;
  std::shared_ptr<Program> currentProgram;

 public:
  Parser(Lexer &lex) : lex(lex) {
    currentProgram = std::make_shared<Program>();
  }

  // Parser entry point.
  // program -> declList
  std::shared_ptr<Program> parseProgram();

  std::shared_ptr<Program> getCurrProgramPtr() const {
    return currentProgram->shared_from_this();
  }

 private:
  // Advance current token from lexer and set currTok
  void advanceCurrent();

  // Get current token kind
  TokenKind currKind() const { return currTok->kind; }

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
  std::unique_ptr<VarDeclAST> parseVarDecl(TokenKind kind, const Identifier &id,
                                           TokenKind modifier);

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
  std::unique_ptr<ReturnStmtAST> parseReturnStmt();

  // Parse an expression that can be evaluated to some value
  // expr -> mutable = expr | mutable += expr |
  //  mutable -= expr | mutable *= expr |
  //  mutable /= expr | mutable++ | mutable-- | simpleExpr
  std::unique_ptr<ExprAST> parseExpr();

  // Expression parsing entry point
  // primary -> numberExpr | identifierExpr | parentheseExpr
  std::unique_ptr<ExprAST> parsePrimaryExpr();

  // Parse a Binary operator expression
  std::unique_ptr<ExprAST> parseBinaryOpExpr(std::unique_ptr<ExprAST> LHS,
                                             Precedence prec);

  // Parse a call expression e.g
  // identifier(expression*)
  std::unique_ptr<CallExprAST> parseCallExpr(Identifier &id);

  // Parse a numeric constant e.g 60
  std::unique_ptr<NumConstAST> parseNumberExpr();

  // Parse a character constant e.g 'a'
  std::unique_ptr<CharConstAST> parseCharExpr();

  // Parse a string literal e.g "string"
  std::unique_ptr<StringLiteralAST> parseStrLiteralExpr();

  // Parse an identifier expression e.g
  // identifier | identifier(expression*)
  std::unique_ptr<ExprAST> parseIdentifierExpr();

  // Parse a expression in parentheses e.g
  // ( expression )
  std::unique_ptr<ExprAST> parseParentheseExpr();
};

#endif  // PARSER_H

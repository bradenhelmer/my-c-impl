// Parser.cpp
// ~~~~~~~~~~
// Parser implementation
#include "Parser.h"

#include <memory>

#include "Common.h"
#include "Token.h"

std::unique_ptr<Program> Parser::parseProgram() {
  // Advance current token to begin parsing.
  advanceCurrent();
  return std::make_unique<Program>(parseDeclList());
}

std::vector<std::unique_ptr<DeclAST>> Parser::parseDeclList() {
  std::vector<std::unique_ptr<DeclAST>> declList;
  while (currTok->kind != eof) {
    std::unique_ptr<DeclAST> decl = parseDecl();
    declList.push_back(std::move(decl));
  }
  return declList;
}

std::unique_ptr<DeclAST> Parser::parseDecl() {
  if (!isPrimitive(currTok->kind))
    return LogError<DeclAST>("Expected type specifer!");
  TokenKind kind = currTok->kind;
  advanceCurrent();
  if (!isIdentifer(currTok->kind))
    return LogError<DeclAST>("Expected identifier!");

  const Identifier id = lex.getIdentifier();

  // If token after identifier is an open parenthese,
  // we know this is a function definition.
  if (currTok->kind == o_paren) {
    advanceCurrent();
    return parseFuncDecl(kind, id);
  } else {
    return parseVarDecl(kind, id);
  }
}

std::unique_ptr<PrototypeAST> Parser::parseProtoType(TokenKind kind,
                                                     const Identifier &id) {
  std::vector<FuncParam> params;
  TokenKind currKind;
  Identifier currId;
  short pos = 0;
  while (currTok->kind != c_paren) {
    if (currTok->kind == comma) advanceCurrent();

    if (!isPrimitive(currTok->kind)) {
      return LogError<PrototypeAST>(
          "Expected type specifer when parsing function arguments!");
    }

    currKind = currTok->kind;
    advanceCurrent();

    if (!isIdentifer(currTok->kind))
      return LogError<PrototypeAST>(
          "Expected identifier when parsing function arguments!");
    currId = lex.getIdentifier();
    params.push_back({.id = currId, .type = currKind, .position = pos});
    pos++;
  }
  advanceCurrent();
  return std::make_unique<PrototypeAST>(kind, id, params);
}

std::unique_ptr<FuncDeclAST> Parser::parseFuncDecl(TokenKind kind,
                                                   const Identifier &id) {
  std::unique_ptr<PrototypeAST> proto = parseProtoType(kind, id);
  if (currTok->kind == semi_colon) {
    advanceCurrent();
    return std::make_unique<FuncDeclAST>(std::move(proto), nullptr);
  } else if (currTok->kind == o_brace) {
    advanceCurrent();
    return std::make_unique<FuncDeclAST>(std::move(proto), parseBlockStmt());
  } else {
    return LogError<FuncDeclAST>("Error parsing function declaration!");
  }
}
std::unique_ptr<VarDeclAST> Parser::parseVarDecl(TokenKind kind,
                                                 const Identifier &id) {
  if (currTok->kind == semi_colon) {
    advanceCurrent();
    return std::make_unique<VarDeclAST>(kind, id, nullptr);
  } else if (currTok->kind == equal) {
    advanceCurrent();
    return std::make_unique<VarDeclAST>(kind, id, parseExpr());
  } else {
    return LogError<VarDeclAST>("Failed to parse variable declaration!");
  }
}

std::unique_ptr<BlockStmtAST> Parser::parseBlockStmt() {
  std::vector<std::unique_ptr<StmtAST>> stmtList;
  while (currTok->kind != c_brace) {
    if (isPrimitive(currTok->kind)) {
      TokenKind currKind = currTok->kind;
      advanceCurrent();
      stmtList.push_back(parseVarDecl(currKind, lex.getIdentifier()));
    } else {
      switch (currTok->kind) {
	case kw_if:
	  stmtList.push_back(parseCondStatement());
	  break;
	case kw_for:
	case kw_while:
	  stmtList.push_back(parseIterStmt());
	  break;
	case kw_return:
	  advanceCurrent();
	  stmtList.push_back(parseReturnStmt());
      }
    }
  }
  advanceCurrent();
  return std::make_unique<BlockStmtAST>(std::move(stmtList));
}

std::unique_ptr<BlockStmtAST> Parser::parseCondStatement() {}

std::unique_ptr<BlockStmtAST> Parser::parseIterStmt() {}

std::unique_ptr<ReturnStmtAST> Parser::parseReturnStmt() {
  if (currTok->kind == semi_colon) {
    advanceCurrent();
    return std::make_unique<ReturnStmtAST>(nullptr);
  }
  return std::make_unique<ReturnStmtAST>(parseExpr());
}

template <typename T>
std::unique_ptr<T> Parser::LogError(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  return nullptr;
}

void Parser::advanceCurrent() {
  lex.advanceToken();
  currTok = lex.getCurrentToken();
}

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
  if (!isTypeKeyword(currTok->kind))
    return LogError<DeclAST>("Expected type specifer!");
  TokenKind kind = currTok->kind;
  advanceCurrent();
  if (!isIdentifer(currTok->kind))
    return LogError<DeclAST>("Expected identifier!");

  const Identifier id = lex.getIdentifier();
  advanceCurrent();

  // If token after identifier is an open parenthese,
  // we know this is a function definition.
  if (currTok->kind == o_paren) {
    return parseFuncDecl(kind, id);
  } else {
    return parseVarDecl(kind, id);
  }
}

std::unique_ptr<FuncDeclAST> Parser::parseFuncDecl(TokenKind kind,
                                                   const Identifier &id) {}

std::unique_ptr<VarDeclAST> Parser::parseVarDecl(TokenKind kind,
                                                 const Identifier &id) {
  if (currTok->kind == semi_colon) {
    advanceCurrent();
    return std::make_unique<VarDeclAST>(kind, id, nullptr);
  } else if (currTok->kind == equal) {
    advanceCurrent();
    return std::make_unique<VarDeclAST>(kind, id, parseExpr());
  }
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

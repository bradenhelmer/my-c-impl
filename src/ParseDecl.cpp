// Declaration Parsing Implementation
#include "Parser.h"

std::vector<std::unique_ptr<DeclAST>> Parser::parseDeclList() {
  std::vector<std::unique_ptr<DeclAST>> declList;
  while (currKind() != eof) {
    declList.push_back(parseDecl());
    advanceCurrent();
  }
  return declList;
}

std::unique_ptr<DeclAST> Parser::parseDecl() {
  if (!isPrimitive(currKind()))
    return LogError<DeclAST>("Expected type specifer!");
  TokenKind kind = currKind();
  advanceCurrent();
  if (!isIdentifer(currKind()))
    return LogError<DeclAST>("Expected identifier!");

  const Identifier id = lex.getIdentifier();

  // If token after identifier is an open parenthese,
  // we know this is a function definition.
  if (currKind() == o_paren) {
    return parseFuncDecl(kind, id);
  } else {
    return parseVarDecl(kind, id);
  }
}

std::unique_ptr<PrototypeAST> Parser::parseProtoType(TokenKind kind,
                                                     const Identifier &id) {
  advanceCurrent();
  std::vector<FuncParam> params;
  TokenKind currParamKind;
  Identifier currParamId;
  short pos = 0;
  while (currKind() != c_paren) {
    if (currKind() == comma) advanceCurrent();

    if (!isPrimitive(currKind())) {
      return LogError<PrototypeAST>(
          "Expected type specifer when parsing function arguments!");
    }

    currParamKind = currKind();
    advanceCurrent();
    if (!isIdentifer(currKind()))

      return LogError<PrototypeAST>(
          "Expected identifier when parsing function arguments!");
    currParamId = lex.getIdentifier();
    params.push_back(
        {.id = currParamId, .type = currParamKind, .position = pos});
    pos++;
  }
  advanceCurrent();
  return std::make_unique<PrototypeAST>(kind, id, params);
}

std::unique_ptr<FuncDeclAST> Parser::parseFuncDecl(TokenKind kind,
                                                   const Identifier &id) {
  std::unique_ptr<PrototypeAST> proto = parseProtoType(kind, id);
  if (currKind() == semi_colon) {
    return std::make_unique<FuncDeclAST>(std::move(proto), nullptr);
  } else if (currKind() == o_brace) {
    return std::make_unique<FuncDeclAST>(std::move(proto), parseBlockStmt());
  } else {
    return LogError<FuncDeclAST>("Error parsing function declaration!");
  }
}
std::unique_ptr<VarDeclAST> Parser::parseVarDecl(TokenKind kind,
                                                 const Identifier &id) {
  if (currKind() == semi_colon) {
    advanceCurrent();
    return std::make_unique<VarDeclAST>(kind, id, nullptr);
  } else if (currKind() == equal) {
    advanceCurrent();
    return std::make_unique<VarDeclAST>(kind, id, parseExpr());
  } else {
    return LogError<VarDeclAST>("Failed to parse variable declaration!");
  }
}

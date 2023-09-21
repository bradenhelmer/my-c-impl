// Declaration Parsing Implementation
#include "Diagnostics.h"
#include "Parser.h"
std::vector<std::unique_ptr<DeclAST>> Parser::parseDeclList() {
  std::vector<std::unique_ptr<DeclAST>> declList;
  while (currKind() != eof) {
    declList.push_back(parseDecl());
    if (currKind() == semi_colon) advanceCurrent();
  }
  return declList;
}

std::unique_ptr<DeclAST> Parser::parseDecl() {
  TokenKind possibleModifier = mut;

  // Check to see if there is a decl modifere e.g 'const'
  if (isModifier(currKind())) {
    possibleModifier = currKind();
    advanceCurrent();
  }

  if (!isPrimitive(currKind())) {
    Diagnostic::runDiagnostic(
        Diagnostic::syntax_error,
        "Expected type specifier when parsing declaration!");
  }

  TokenKind kind = currKind();
  advanceCurrent();
  if (!isIdentifier(currKind()))
    Diagnostic::runDiagnostic(Diagnostic::syntax_error,
                              "Expected identifier when parsing declaration!");

  const Identifier id = lex.getIdentifier();

  // If token after identifier is an open parenthese,
  // we know this is a function definition.
  if (currKind() == o_paren) {
    return parseFuncDecl(kind, id);
  } else {
    return parseVarDecl(kind, id, possibleModifier);
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
      Diagnostic::runDiagnostic(
          Diagnostic::syntax_error,
          "Expected type specifier when parsing function arguments!");
    }

    currParamKind = currKind();
    advanceCurrent();
    if (!isIdentifier(currKind()))
      Diagnostic::runDiagnostic(
          Diagnostic::syntax_error,
          "Expected identifier when parsing function arguments!");

    currParamId = lex.getIdentifier();
    params.push_back(
        {.id = currParamId, .type = currParamKind, .position = pos});
    pos++;
  }
  advanceCurrent();
  return std::make_unique<PrototypeAST>(getCurrProgramPtr(), kind, id, params);
}

std::unique_ptr<FuncDeclAST> Parser::parseFuncDecl(TokenKind kind,
                                                   const Identifier &id) {
  std::unique_ptr<PrototypeAST> proto = parseProtoType(kind, id);
  if (currKind() == semi_colon) {
    return std::make_unique<FuncDeclAST>(getCurrProgramPtr(), std::move(proto),
                                         nullptr);
  } else if (currKind() == o_brace) {
    return std::make_unique<FuncDeclAST>(getCurrProgramPtr(), std::move(proto),
                                         parseBlockStmt());
  } else {
    Diagnostic::runDiagnostic(Diagnostic::parse_error,
                              "Error parsing function declaration!");
  }
  return nullptr;
}
std::unique_ptr<VarDeclAST> Parser::parseVarDecl(TokenKind kind,
                                                 const Identifier &id,
                                                 TokenKind modifier) {
  if (currKind() == semi_colon) {
    advanceCurrent();
    if (modifier == kw_const)
      Diagnostic::runDiagnostic(Diagnostic::invalid_modifier_error,
                                "Variables declared as const must be "
                                "initialized with a valid expression!");
    return std::make_unique<VarDeclAST>(getCurrProgramPtr(), kind, id, nullptr);
  } else if (currKind() == equal) {
    advanceCurrent();
    if (modifier == kw_const)
      return std::make_unique<ConstVarDeclAST>(getCurrProgramPtr(), kind, id,
                                               parseExpr());
    return std::make_unique<VarDeclAST>(getCurrProgramPtr(), kind, id,
                                        parseExpr());
  } else {
    Diagnostic::runDiagnostic(Diagnostic::parse_error,
                              "Error parsing variable declaration!");
    return nullptr;
  }
}

// Token header file
#ifndef TOKEN_H
#define TOKEN_H
#include <algorithm>
#include <map>
#include <string>

// Token enums
enum TokenKind : unsigned short {
#define TOKEN(X) X,
#include "TokenDefs.h"
  NUM_TOKENS
};

static const std::string TokenNames[NUM_TOKENS] = {
#define TOKEN(X) #X,
#include "TokenDefs.h"
};

static const std::map<std::string, TokenKind> keywords = {
#define TOKEN_KEYWORD(X) {#X, kw_##X},
#include "TokenDefs.h"
};

typedef struct {
  TokenKind kind;
  char *start;
  char *end;
  unsigned length;
} Token;

static bool isPrintable(TokenKind kind) {
  return kind == numeric_literal || kind == identifier;
}
static const std::string getTokenName(TokenKind kind) {
  return TokenNames[kind];
}

static bool isKeyword(std::string ident) {
  auto kw = keywords.find(ident);
  return kw != keywords.end();
}

static TokenKind getKeywordToken(std::string ident) {
  return keywords.at(ident);
}

#endif // TOKEN_H

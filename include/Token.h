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

static const std::map<std::string, TokenKind> typeKeywords = {
#define TOKEN_KEYWORD_TYPE(X) {#X, kwt_##X},
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

static bool isTypeKeyword(std::string ident) {
  auto kw = typeKeywords.find(ident);
  return kw != typeKeywords.end();
}

static bool isTypeKeyword(TokenKind kind) {
  switch(kind) {
    default: return false;
    case kwt_char:
    case kwt_int: return true;
  } 
}

static TokenKind getTypeKeywordToken(std::string ident) {
  return typeKeywords.at(ident);
}

#endif // TOKEN_H

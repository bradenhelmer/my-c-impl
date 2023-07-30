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

typedef struct {
  std::string idStr;
  bool isArray;
  unsigned size;
} Identifier;

typedef struct {
  Identifier id;
  TokenKind type;
  short position;
} FuncParam;

inline static bool isPrintable(TokenKind kind) {
  return kind == num_const || kind == identifier;
}

inline static const std::string getTokenName(TokenKind kind) {
  return TokenNames[kind];
}

inline static bool isKeyword(std::string ident) {
  auto kw = keywords.find(ident);
  return kw != keywords.end();
}

inline static bool isIdentifer(TokenKind kind) { return kind == identifier; }

inline static bool isPrimitive(TokenKind kind) {
  switch (kind) {
    case kw_bool:
    case kw_char:
    case kw_int:
    case kw_void:
      return true;
    default:
      return false;
  }
}

inline static const std::string getPrimitiveName(TokenKind kind) {
  if (!isPrimitive(kind)) {
    return NULL;
  } else {
    return getTokenName(kind).substr(3);
  }
}

inline static TokenKind getKeywordToken(std::string ident) {
  return keywords.at(ident);
}

#endif  // TOKEN_H

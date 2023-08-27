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

static const char *getPunctuatorSpelling(TokenKind kind) {
  switch (kind) {
#define TOKEN_PUNCTUATOR(X, Y) \
  case X:                      \
    return Y;
#include "TokenDefs.h"
    default:
      break;
  }
  return nullptr;
}

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

inline static bool isIdentifier(TokenKind kind) { return kind == identifier; }

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

inline static bool isAssignmentOperator(TokenKind kind) {
  switch (kind) {
    case equal:
    case plus_equal:
    case minus_equal:
    case star_equal:
    case slash_equal:
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

enum Precedence {
  error = -1,
  base = 0,
  assignment = 1,
  logical_or = 2,
  loglical_and = 3,
  equality = 4,
  relational = 5,
  additive = 6,
  multiplicative = 7,
  exponential = 8,
};

static Precedence getOperatorPrecedence(TokenKind kind) {
  switch (kind) {
    case equal:
    case plus_equal:
    case minus_equal:
    case star_equal:
    case slash_equal:
      return assignment;
    case pipe_pipe:
      return logical_or;
    case amp_amp:
      return loglical_and;
    case equal_equal:
    case bang_equal:
      return equality;
    case less:
    case greater:
    case less_equal:
    case greater_equal:
      return relational;
    case plus:
    case minus:
      return additive;
    case slash:
    case star:
    case percent:
      return multiplicative;
    case hat:
      return exponential;
    default:
      return error;
  }
}

#define PRINT_TOKEN PRINT(getTokenName(currKind()))

#endif  // TOKEN_H

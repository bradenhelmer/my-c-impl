// Token header file
#ifndef TOKEN_H
#define TOKEN_H
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

typedef struct {
  TokenKind kind;
  char *start;
  char *end;
  unsigned length;
} Token;

static bool isPrintable(TokenKind kind) {
  return kind == NUMERIC_LITERAL || kind == STRING_LITERAL ||
         kind == IDENTIFIER;
}
static const std::string getTokenName(TokenKind kind) {
  return TokenNames[kind];
}

#endif // TOKEN_H

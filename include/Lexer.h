// Lexer.h
// ~~~~~~~
// Lexer definition
#include "Token.h"

class Lexer {
public:
  static Token currentToken;
  void advanceToken();
  Lexer(char *buffer);

private:
  char *buffer;
  void lexNumericLiteral();
  void lexStringLiteral();
};

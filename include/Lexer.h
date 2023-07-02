// Lexer.h
// ~~~~~~~
// Lexer definition
#include <vector>

#include "Token.h"

class Lexer {
public:
  void advanceToken();
  Token getCurrentToken() const { return currentToken; }
  void lexAndPrintTokens();
  void printToken(int num);
  Lexer(std::vector<char> *buffer);

private:
  Token currentToken;
  std::vector<char> *buffer;
  void lexNumericLiteral();
  void lexIdentifier();
  void skipWhiteSpace();
  char *bufPtr;
  static bool isWhiteSpace(const char c) {
    return c == '\t' || c == ' ' || c == '\v' || c == '\f';
  }
};

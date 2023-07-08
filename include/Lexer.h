// Lexer.h
// ~~~~~~~
// Lexer definition
#ifndef LEXER_H
#define LEXER_H
#include <vector>

#include "Token.h"

class Lexer {
public:
  void advanceToken();
  Token *getCurrentToken() { return &currentToken; }
  void lexAndPrintTokens();
  void printToken(int num);
  std::string getIdentifier();
  double getNumericLiteral();
  Lexer(std::vector<char> *buffer);

private:
  Token currentToken;
  std::vector<char> *buffer;
  void lexNumericLiteral();
  void lexIdentifier();
  void skipWhiteSpace();
  char *bufPtr;
  static bool isWhiteSpace(const char c) {
    return c == '\t' || c == ' ' || c == '\v' || c == '\f' || c == '\n';
  }
};

#endif // LEXER_H

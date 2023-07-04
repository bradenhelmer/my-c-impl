// Lexer.cpp
// ~~~~~~~~~
// Lexer implementation
#include "Lexer.h"

#include <iostream>

#include "Common.h"

Lexer::Lexer(std::vector<char> *buffer) : buffer(buffer) {
  currentToken = {
      .kind = unknown,
      .start = nullptr,
      .end = nullptr,
      .length = 0,
  };
  bufPtr = &*buffer->begin();
}

void Lexer::advanceToken() {
  if (isWhiteSpace(*bufPtr)) {
    do {
      bufPtr++;
    } while (isWhiteSpace(*bufPtr));
  }
  currentToken.start = bufPtr;
  currentToken.end = bufPtr;
  switch (*bufPtr) {
  case 0:
    if (bufPtr == &*buffer->end()) {
      currentToken.kind = eof;
      break;
    }
  case '(':
    currentToken.kind = o_paren;
    break;
  case ')':
    currentToken.kind = c_paren;
    break;
  case '{':
    currentToken.kind = o_brace;
    break;
  case '}':
    currentToken.kind = c_brace;
    break;
  case '+':
    currentToken.kind = add;
    break;
  case '-':
    currentToken.kind = sub;
    break;
  case '*':
    currentToken.kind = mult;
    break;
  case '/':
    currentToken.kind = divide;
    break;
  case '%':
    currentToken.kind = modulo;
    break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    currentToken.kind = numeric_literal;
    break;
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'S':
  case 'T':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
  case '_':
    currentToken.kind = identifier;
    break;
  default:
    currentToken.kind = unknown;
    break;
  }
  if (currentToken.kind == numeric_literal) {
    return lexNumericLiteral();
  } else if (currentToken.kind == identifier) {
    return lexIdentifier();
  } else {
    bufPtr++;
  }
}

void Lexer::lexNumericLiteral() {
  while (isdigit(*bufPtr)) {
    bufPtr++;
    currentToken.length++;
  }
  currentToken.end = bufPtr - 1;
}

void Lexer::lexIdentifier() {
  while (isalpha(*bufPtr) || *bufPtr == '_') {
    bufPtr++;
    currentToken.length++;
  }
  currentToken.end = bufPtr - 1;
  std::string ident(currentToken.start, currentToken.end + 1);
  if (isKeyword(ident)) {
    currentToken.kind = getKeywordToken(ident);
  }
}

void Lexer::lexAndPrintTokens() {
  std::cout << "Lexing Tokens\n-------------\n";
  advanceToken();
  int count = 1;
  while (currentToken.kind != eof) {
    printToken(count++);
    advanceToken();
  }
  printToken(count);
}

void Lexer::printToken(int num) {
  std::cout << num << ". ";
  if (isPrintable(currentToken.kind)) {
    for (char *curr = currentToken.start; curr <= currentToken.end; curr++) {
      std::cout << *curr;
    }
    std::cout << std::endl;
  } else {
    PRINT(getTokenName(currentToken.kind));
  }
}

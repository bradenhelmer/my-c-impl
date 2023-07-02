// Lexer.cpp
// ~~~~~~~~~
// Lexer implementation
#include "Lexer.h"

#include <iostream>

#include "Common.h"

Lexer::Lexer(std::vector<char> *buffer) : buffer(buffer) {
  currentToken = {
      .kind = UNKNOWN,
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
      currentToken.kind = EOF;
      break;
    }
  case '(':
    currentToken.kind = O_PAREN;
    break;
  case ')':
    currentToken.kind = C_PAREN;
    break;
  case '{':
    currentToken.kind = O_BRACE;
    break;
  case '}':
    currentToken.kind = C_BRACE;
    break;
  case '+':
    currentToken.kind = ADD;
    break;
  case '-':
    currentToken.kind = SUB;
    break;
  case '*':
    currentToken.kind = MULT;
    break;
  case '/':
    currentToken.kind = DIV;
    break;
  case '%':
    currentToken.kind = MODULO;
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
    currentToken.kind = NUMERIC_LITERAL;
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
    currentToken.kind = IDENTIFIER;
    break;
  default:
    currentToken.kind = UNKNOWN;
    break;
  }
  if (currentToken.kind == NUMERIC_LITERAL) {
    return lexNumericLiteral();
  } else if (currentToken.kind == IDENTIFIER) {
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
}

void Lexer::lexAndPrintTokens() {
  std::cout << "Lexing Tokens\n-------------\n";
  advanceToken();
  int count = 1;
  while (currentToken.kind != EOF) {
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

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
  const char peek = *(bufPtr + 1);
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
    case '[':
      currentToken.kind = o_bracket;
      break;
    case ']':
      currentToken.kind = c_bracket;
      break;
    case ';':
      currentToken.kind = semi_colon;
      break;
    case ',':
      currentToken.kind = comma;
      break;
    case '"':
      currentToken.kind = quote;
      break;
    case '\'':
      currentToken.kind = apost;
      break;
    case '=':
      if (*(bufPtr + 1) == '=') {
	currentToken.kind = equal_equal;
	bufPtr++;
      } else {
	currentToken.kind = equal;
      }
      break;
    case '+':
      if (peek == '=') {
	currentToken.kind = plus_equal;
	bufPtr++;
      } else if (peek == '+') {
	currentToken.kind = plus_plus;
      } else {
	currentToken.kind = plus;
      }
      break;
    case '-':
      if (*(bufPtr + 1) == '=') {
	currentToken.kind = minus_equal;
	bufPtr++;
      } else if (peek == '-') {
	currentToken.kind = minus_minus;
      } else {
	currentToken.kind = minus;
      }
      break;
    case '*':
      if (*(bufPtr + 1) == '=') {
	currentToken.kind = star_equal;
	bufPtr++;
      } else {
	currentToken.kind = star;
      }
      break;
    case '/':
      if (peek == '=') {
	currentToken.kind = slash_equal;
	bufPtr++;
      } else {
	currentToken.kind = slash;
      }
      break;
    case '%':
      currentToken.kind = percent;
      break;
    case '|':
      if (peek == '|') {
	currentToken.kind = pipe_pipe;
	bufPtr++;
      } else {
	currentToken.kind = unknown;
      }
      break;
    case '&':
      if (peek == '&') {
	currentToken.kind = amp_amp;
	bufPtr++;
      } else {
	currentToken.kind = unknown;
      }
      break;
    case '!':
      currentToken.kind = bang;
      break;
    case '<':
      if (peek == '=') {
	currentToken.kind = less_equal;
      } else {
      }
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
      currentToken.kind = num_const;
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
  if (currentToken.kind == num_const) {
    return lexNumericLiteral();
  } else if (currentToken.kind == identifier) {
    return lexIdentifier();
  } else {
    currentToken.end = bufPtr;
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
  do {
    bufPtr++;
    currentToken.length++;
  } while (isalpha(*bufPtr) || *bufPtr == '_');
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

Identifier Lexer::getIdentifier() {
  Identifier id = {.idStr = std::string(currentToken.start, currentToken.end),
                   .isArray = false};
  advanceToken();

  // Lex array id
  if (currentToken.kind == o_bracket) {
    advanceToken();
    ASSERT(currentToken.kind == num_const);
    id.size = getNumericLiteral();
    advanceToken();
    ASSERT(currentToken.kind == c_bracket);
  }
  return id;
}

double Lexer::getNumericLiteral() {
  return std::stod(std::string(currentToken.start, currentToken.end + 1));
}

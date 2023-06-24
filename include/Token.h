// Token header file
#include <string>

// Token enums
enum TokenKind : unsigned short {
    #define TOKEN(X) X,
    #include "TokenDefs.h"
    NUM_TOKENS
};

std::string TokenNames[NUM_TOKENS] = {
  #define TOKEN(X) #X,
  #include "TokenDefs.h"
};


typedef struct {
  TokenKind kind;
  char *start;
  char *end;
  unsigned length;
} Token;

#include <iostream>
#include "Token.h"

int main(void) {
  for (int i = 0; i < NUM_TOKENS; i++) {
    std::cout << TokenNames[i] << std::endl; 
  }
  return 0;
}

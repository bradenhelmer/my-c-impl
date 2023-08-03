// LLVM related functions and items
#include <llvm/IR/Type.h>

#include "Token.h"

#define CHAR_BIT_LENGTH 8
#define INT_BIT_LENGTH 32

llvm::IntegerType *getTypeFromPrimitive(llvm::LLVMContext &context,
                                               TokenKind kind);

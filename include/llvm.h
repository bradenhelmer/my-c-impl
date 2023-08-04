// LLVM related functions and items
#ifndef LANG_LLVM_H
#define LANG_LLVM_H
#include <llvm/IR/Type.h>

#include <functional>
#include <unordered_map>

#include "Token.h"

#define CHAR_BIT_LENGTH 8
#define INT_BIT_LENGTH 32

static llvm::Type *getVoidType(llvm::LLVMContext &context) {
  return llvm::Type::getVoidTy(context);
}

llvm::IntegerType *getIntType(llvm::LLVMContext &context, TokenKind kind);

#endif  // LANG_LLVM_H

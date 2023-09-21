// LLVM related functions and items
#ifndef LANG_LLVM_H
#define LANG_LLVM_H
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>

#include <functional>
#include <unordered_map>

#include "Token.h"

#define CHAR_BIT_LENGTH 8
#define INT_BIT_LENGTH 32

bool checkExpectedType(llvm::Type *expected, llvm::Type *evaluated,
                       TokenKind primitive);

#endif  // LANG_LLVM_H

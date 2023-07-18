// Common.h
// ~~~~~~~~
// Common macro definitions for the toolchain.
#ifndef COMMON_H
#define COMMON_H
#include <llvm/Support/raw_ostream.h>

#include <cassert>
#include <iostream>

#define ASSERT(X) assert(X)
#define PRINT(X) std::cout << X << std::endl

#define INDENT_CHAR '\t'
#define LLVM_OUT(X) llvm::outs() << X
#define LLVM_OUT_NL(X) \
  llvm::outs() << X;   \
  llvm::outs() << '\n';

#endif  // COMMON_H

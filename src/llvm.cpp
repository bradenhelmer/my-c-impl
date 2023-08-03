// LLVM related function implementations
#include "llvm.h"

llvm::IntegerType *getTypeFromPrimitive(llvm::LLVMContext &context,
                                        TokenKind kind) {
  switch (kind) {
    case kw_int:
      return llvm::Type::getInt32Ty(context);
    case kw_char:
      return llvm::Type::getInt8Ty(context);
  }
}

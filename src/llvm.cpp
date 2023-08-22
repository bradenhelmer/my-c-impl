// LLVM related function implementations
#include "llvm.h"

llvm::IntegerType *getIntType(llvm::LLVMContext &context, TokenKind kind) {
  switch (kind) {
    case kw_int:
      return llvm::Type::getInt32Ty(context);
    case kw_char:
      return llvm::Type::getInt8Ty(context);
    case kw_bool:
      return llvm::Type::getInt1Ty(context);
    default:
      return nullptr;
  }
}

llvm::LoadInst *loadGlobal(llvm::Value *val, llvm::IRBuilder<> &builder) {
  llvm::GlobalVariable *G = llvm::cast<llvm::GlobalVariable>(val);
  return builder.CreateLoad(G->getValueType(), G);
}

// Class definition for the JIT compiler
#ifndef JIT_HH
#define JIT_HH

#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>

#include "Ast.h"

class JIT {
  // Initialize Native evionment Items
  void initEnvironment() const {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
  }

  Program &program;

 public:
  JIT(Program &program) : program(program) { initEnvironment(); }
};

#endif

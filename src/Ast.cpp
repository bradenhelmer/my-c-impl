// Base AST implementations
#include "Ast.h"

#include <llvm/IR/Function.h>
#include <llvm/Pass.h>

Program::Program() {
  ctx = std::make_unique<llvm::LLVMContext>();
  module = std::make_unique<llvm::Module>("main", getContext());
  builder = std::make_unique<llvm::IRBuilder<>>(getContext());
  initAnalysisManagers();
  initPassManagers();
}

void Program::initAnalysisManagers() {
  LAM = std::make_unique<llvm::LoopAnalysisManager>();
  FAM = std::make_unique<llvm::FunctionAnalysisManager>();
  CGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
  MAM = std::make_unique<llvm::ModuleAnalysisManager>();
}

void Program::initPassManagers() {
  llvm::PassBuilder PB;

  PB.registerLoopAnalyses(getLoopAnalysisManager());
  PB.registerFunctionAnalyses(getFunctionAnalysisManager());
  PB.registerCGSCCAnalyses(getCGSCCAnalysisManager());
  PB.registerModuleAnalyses(getModuleAnalysisManager());
  PB.crossRegisterProxies(
      getLoopAnalysisManager(), getFunctionAnalysisManager(),
      getCGSCCAnalysisManager(), getModuleAnalysisManager());

  MPM = std::make_unique<llvm::ModulePassManager>(
      PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2));

  FPM = std::make_unique<llvm::FunctionPassManager>(
      PB.buildFunctionSimplificationPipeline(llvm::OptimizationLevel::O2,
                                             llvm::ThinOrFullLTOPhase::None));
}

llvm::Value *Program::codeGen() {
  for (const auto &decl : declList) {
    decl->codeGen();
  }
  runDefaultOptimization();
  return nullptr;
}

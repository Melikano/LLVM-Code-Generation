#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h" // For errs().

#include "passWithNewPM.h"

using namespace llvm;

extern bool solutionConstantPropagation(llvm::Function &);

PreservedAnalyses YourTurnConstantPropagationNewPass::run(Function &fun, FunctionAnalysisManager &fam)
{
    bool hasChanged = solutionConstantPropagation(fun);

    if (!hasChanged)
        return PreservedAnalyses::all();

    PreservedAnalyses pa;
    return pa;
}

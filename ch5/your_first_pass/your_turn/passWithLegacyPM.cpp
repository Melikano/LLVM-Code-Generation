#include "llvm/IR/Function.h"
#include "llvm/Pass.h"          // For FunctionPass & INITIALIZE_PASS.
#include "llvm/Support/Debug.h" // For errs().

using namespace llvm;

extern bool solutionConstantPropagation(llvm::Function &);

// The implementation of this function is generated at the end of this file. See
// INITIALIZE_PASS.
namespace llvm
{
  void initializeYourTurnConstantPropagationPass(PassRegistry &);
};

namespace
{
  class YourTurnConstantPropagation : public FunctionPass
  {
  public:
    static char ID;
    YourTurnConstantPropagation() : FunctionPass(ID)
    {
      initializeYourTurnConstantPropagationPass(*PassRegistry::getPassRegistry());
    }

    bool runOnFunction(Function &fun) override
    {
      return solutionConstantPropagation(fun);
    }
  };
} // End anonymous namespace.

char YourTurnConstantPropagation::ID = 0;

INITIALIZE_PASS(YourTurnConstantPropagation, "my-cp-legacy", "My Constant Propagation with Legacy Pass", false, false);

Pass *createYourTurnPassForLegacyPM()
{
  Pass *myPass = new YourTurnConstantPropagation();
  return myPass;
}



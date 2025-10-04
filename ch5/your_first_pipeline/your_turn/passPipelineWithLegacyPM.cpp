#include "llvm/IR/LegacyPassManager.h" // For legacy::PassManager.
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/IPO/AlwaysInliner.h"
using namespace llvm;

void runYourTurnPassPipelineForLegacyPM(Module &MyModule) {

    legacy::PassManager LPM;

    FunctionPass *mem2regPass = createPromoteMemoryToRegisterPass();
    LPM.add(mem2regPass);

    FunctionPass *instCombinePass = createInstructionCombiningPass();
    LPM.add(instCombinePass);

    Pass *alwaysInline = createAlwaysInlinerLegacyPass();
    LPM.add(alwaysInline);

    LPM.run(MyModule);
    
}

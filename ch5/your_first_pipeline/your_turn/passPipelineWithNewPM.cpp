#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h" // For the new PassManager.
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Transforms/IPO/AlwaysInliner.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"

using namespace llvm;

void runYourTurnPassPipelineForNewPM(Module &MyModule)
{

    LLVMContext &CTX = MyModule.getContext();

    FunctionAnalysisManager FAM;
    ModuleAnalysisManager MAM;

    PassInstrumentationCallbacks PIC;
    PrintPassOptions PrintPassOpts;
    PrintPassOpts.Verbose = true;
    PrintPassOpts.SkipAnalyses = false;
    PrintPassOpts.Indent = true;
    StandardInstrumentations SI(CTX, true, false, PrintPassOpts);
    SI.registerCallbacks(PIC, &MAM);

    MAM.registerPass([&]
                     { return PassInstrumentationAnalysis(&PIC); });
    FAM.registerPass([&]
                     { return PassInstrumentationAnalysis(&PIC); });

    MAM.registerPass([&]
                     { return FunctionAnalysisManagerModuleProxy(FAM); });
    FAM.registerPass([&]
                     { return ModuleAnalysisManagerFunctionProxy(MAM); });

    ModulePassManager ModPM;
    FunctionPassManager FunPM;

    PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);
    PB.registerModuleAnalyses(MAM);

    FunPM.addPass(PromotePass());
    FunPM.addPass(InstCombinePass());
    ModPM.addPass(createModuleToFunctionPassAdaptor(std::move(FunPM)));

    ModPM.addPass(AlwaysInlinerPass());
    ModPM.run(MyModule, MAM);
}

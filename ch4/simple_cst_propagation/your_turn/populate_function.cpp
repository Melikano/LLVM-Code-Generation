#include "llvm/ADT/PostOrderIterator.h" // For ReversePostOrderTraversal.
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/MachineFrameInfo.h" // For CreateStackObject.
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h" // For MachinePointerInfo.
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetOpcodes.h"     // For INLINEASM.
#include "llvm/CodeGenTypes/LowLevelType.h" // For LLT.
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h" // For BinaryOperator, etc.
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h" // For errs().

using namespace llvm;

bool replace(Instruction &Instr, Value *V)
{
  if (!V)
    return false;
  Instr.replaceAllUsesWith(V);
  Instr.eraseFromParent();
  return true;
}

// Takes \p Foo and apply a simple constant propagation optimization.
// \returns true if \p Foo was modified (i.e., something had been constant
// propagated), false otherwise.
bool myConstantPropagation(Function &Foo)
{
  // TODO: populate this function.

  if (Foo.empty())
    return false;

  ReversePostOrderTraversal<Function *> RPOT_Foo(&Foo);
  bool hasChanged = false;
  LLVMContext &CTX = Foo.getParent()->getContext();
  for (BasicBlock *BB : RPOT_Foo)
  {
    for (Instruction &Instr : make_early_inc_range(*BB))
    {
      if (isa<BinaryOperator>(Instr))
      {
        ConstantInt *Op0 = dyn_cast<ConstantInt>(Instr.getOperand(0));
        ConstantInt *Op1 = dyn_cast<ConstantInt>(Instr.getOperand(1));
        if (!Op0 || !Op1)
        {
          continue;
        }
        const APInt &Op0Const = Op0->getValue();
        const APInt &Op1Const = Op1->getValue();
        switch (Instr.getOpcode())
        {
        case Instruction::Add:
          hasChanged = replace(Instr, ConstantInt::get(CTX, Op0Const + Op1Const));
          break;
        case Instruction::Sub:
          hasChanged = replace(Instr, ConstantInt::get(CTX, Op0Const - Op1Const));
          break;
        case Instruction::Mul:
          hasChanged = replace(Instr, ConstantInt::get(CTX, Op0Const * Op1Const));
          break;
        case Instruction::SDiv:
          hasChanged = replace(Instr, Op1Const.isZero() ? nullptr : ConstantInt::get(CTX, Op0Const.sdiv(Op1Const)));
          break;
        case Instruction::UDiv:
          hasChanged = replace(Instr, Op1Const.isZero() ? nullptr : ConstantInt::get(CTX, Op0Const.udiv(Op1Const)));
          break;
        case Instruction::Shl:
          hasChanged = replace(Instr, ConstantInt::get(CTX, Op0Const.shl(Op1Const)));
          break;
        case Instruction::LShr:
          hasChanged = replace(Instr, ConstantInt::get(CTX, Op0Const.lshr(Op1Const)));
          break;
        case Instruction::AShr:
          hasChanged = replace(Instr, ConstantInt::get(CTX, Op0Const.ashr(Op1Const)));
          break;
        default:
          break;
        }
      }
    }
  }

  return hasChanged;
}

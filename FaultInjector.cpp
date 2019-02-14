//===----------------------------------------------------------------------===//
//
//                       LLVM Fault Injection Tool
//
//===----------------------------------------------------------------------===//
//
//  Copyright (C) 2019. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Dependency.h"
#include <set>
#include <string>

#define DEBUG_TYPE "fault-injection"

using namespace llvm;

//static cl::opt<bool>
//FaultInjector("fault-injection", cl::init(false), cl::NotHidden,
//  cl::desc("Fault Injection"));

namespace {
  
///---------------------------------------------------------
///
///                    Insert Machine
///
///---------------------------------------------------------

class FaultInjectionInsertMachine {
public:

  static void insertInitFunc(Module &M, int count_of_index) {

    Function *mainFunc = M.getFunction("main");
    BasicBlock *entryBB = &mainFunc->front();

    //
    // Insert 'fault_inject_init' function.
    //
    Constant *initFunc = getInitFunction(M);
    std::vector<Value*> initFunc_args(1);
    initFunc_args[0] = ConstantInt::get(Type::getInt32Ty(M.getContext()), count_of_index);
    ArrayRef<Value*> initFunc_args_array_ref(initFunc_args);
    CallInst::Create(initFunc, initFunc_args_array_ref, "", entryBB->getFirstNonPHI());

    //
    // Insert 'fault_inject_finish' function.
    //
    Constant *finishFunc = getFinishFunction(M);

    std::set<Instruction*> exitInsts;
    getProgramExitInsts(M, exitInsts);
    assert(exitInsts.size() != 0
      && "Program does not have explicit exit point");
    for (std::set<Instruction*>::iterator it = exitInsts.begin();
      it != exitInsts.end(); ++it) {
      Instruction *term = *it;
      CallInst::Create(finishFunc, "", term);
    }

    createInjectionFunctions(M);
  }

private:

  static Constant * getInitFunction(Module &M) {
    LLVMContext &context = M.getContext();

    std::vector<Type*> fault_inject_init_param_types(1);
    fault_inject_init_param_types[0] = Type::getInt32Ty(context);
    ArrayRef<Type*> fault_inject_init_param_types_array_ref(fault_inject_init_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(Type::getVoidTy(context),
      fault_inject_init_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject_init", fi_init_func_type);
  }

  static Constant * getFinishFunction(Module &M) {
    LLVMContext &context = M.getContext();

    FunctionType *fi_init_func_type = FunctionType::get(Type::getVoidTy(context), false);
    return M.getOrInsertFunction("fault_inject_finish", fi_init_func_type);
  }

  static Instruction *getTermInstofFunction(Function *func) {
    BasicBlock &termbb = func->back();
    Instruction *ret = termbb.getTerminator();

    assert(isa<ReturnInst>(ret) || isa<UnreachableInst>(ret) &&
      "Last instruction is not return or exit() instruction");
    return ret;
  }

  static void getProgramExitInsts(Module &M, std::set<Instruction*> &exitinsts)
  {
    for (Module::iterator m_it = M.begin(); m_it != M.end(); ++m_it) {
      if (!m_it->isDeclaration()) {
        for (inst_iterator f_it = inst_begin(*m_it); f_it != inst_end(*m_it); ++f_it) {
          Instruction *inst = &(*f_it);
          if (CallInst *ci = dyn_cast<CallInst>(inst)) {
            Function *calledFunc = ci->getCalledFunction();
            if (calledFunc && calledFunc->hasName() &&
              calledFunc->getName().str() == "exit") {
              exitinsts.insert(inst);
            }
          }
        }
      }
    }

    Function* mainfunc = M.getFunction("main");
    exitinsts.insert(getTermInstofFunction(mainfunc));
  }

  static void createInjectionFunctions(Module &M) {
    Constant *pre_fi_func = getDetermineFunc(M);
    Constant *injectfunc = getInjectFunc(M);
    Constant *profilefunc = getProfileFunc(M);

    //for (std::map<const Type*, std::string>::const_iterator fi =
    //  fi_rettype_funcname_map.begin();
    //  fi != fi_rettype_funcname_map.end(); ++fi) {
    //  const Type *fi_type = fi->first;
    //  
    //  Type *fi_type_unconst = const_cast<Type*>(fi_type);
    //  
    //  std::string fi_name = fi->second;
    //  createInjectionFuncforType(M, fi_type_unconst, fi_name, injectfunc, pre_fi_func);
    //}
  }

  static Constant *getProfileFunc(Module &M) {
    std::vector<Type*> profile_func_param_types(3);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);
    profile_func_param_types[1] = Type::getInt32Ty(context);
    profile_func_param_types[2] = Type::getInt32Ty(context);

    ArrayRef<Type*> profile_func_param_types_array_ref(profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(Type::getVoidTy(context),
      profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject_profile", fi_init_func_type);
  }

  static Constant *getDetermineFunc(Module &M) {
    std::vector<Type*> profile_func_param_types(3);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);
    profile_func_param_types[1] = Type::getInt32Ty(context);
    profile_func_param_types[2] = Type::getInt32Ty(context);

    ArrayRef<Type*> profile_func_param_types_array_ref(profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(Type::getInt32Ty(context),
      profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject_determine", fi_init_func_type);
  }

  static Constant *getInjectFunc(Module &M) {
    std::vector<Type*> profile_func_param_types(4);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);
    profile_func_param_types[1] = Type::getInt32Ty(context);
    profile_func_param_types[2] = Type::getInt32Ty(context);
    profile_func_param_types[3] = PointerType::get(Type::getInt8Ty(context), 0);

    ArrayRef<Type*> profile_func_param_types_array_ref(profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(Type::getInt32Ty(context),
      profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject", fi_init_func_type);
  }
};

///---------------------------------------------------------
///
///                   Profile Collector
///
///---------------------------------------------------------

class FaultInjectionTargetSelector {
  Function *target_function;

public:
  FaultInjectionTargetSelector(Function *TargetFunction)
    : target_function(TargetFunction) {
  }

  void run() {

  }

private:

  int getDepedencyLevel(Instruction *I) {
    auto mgr = getInfoManager(I->getDebugLoc()->getLine());
    if (mgr == nullptr)
      return -1;
    mgr->doFolding();
    return (*mgr->begin())->getType();
  }
};

class ProfileCollector {
  std::set<std::string> function_name;

public:
  static ProfileCollector * Instance;

  ProfileCollector(Module &M) {
    getModuleFuncs(M);
  }

  static ProfileCollector *getInstance(Module &M) {
    if (Instance == nullptr)
      Instance = new ProfileCollector(M);
    return Instance;
  }

  void getModuleFuncs(Module &M) {
    for (Module::iterator it = M.begin(); it != M.end(); ++it) {
      std::string func_name = it->getName().str();
      //std::string final_name = demangleFuncName(func_name);

      function_name.insert(func_name);
    }
  }
};

///---------------------------------------------------------
///
///               LLVM Fault Injection Pass
///
///---------------------------------------------------------

struct LLVMFaultInjectionPass : public ModulePass {
  static char ID;

  LLVMFaultInjectionPass() : ModulePass(ID) {
  }

  ~LLVMFaultInjectionPass() {  }

  bool runOnModule(Module &M) override {
    if (!M.getFunction("main")) {
      errs() << "Error: main function not found.";
      exit(1);
    }

    FaultInjectionInsertMachine::insertInitFunc(M, 999);

    return true;
  }

};

} // namespace

char LLVMFaultInjectionPass::ID = 0;

//FunctionPass *myPass = nullptr;
//
//FunctionPass *llvm::createLLVMFaultInjectionPass() {
//  if (myPass == nullptr)
//    myPass = new LLVMFaultInjectionPass();
//  return myPass;
//}

static RegisterPass<LLVMFaultInjectionPass> X("faultinject", "Fault Injection Pass",
  false /* Only looks at CFG */,
  false /* Analysis Pass */);
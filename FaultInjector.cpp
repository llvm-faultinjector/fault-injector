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
#include <sstream>

#define DEBUG_TYPE "fault-injection"

using namespace llvm;

namespace {
  
///---------------------------------------------------------
///
///                    Insert Machine
///
///---------------------------------------------------------

class FaultInjectionInsertMachine {
public:

  static void insertMetaFunc(Module &M, int count_of_index) {

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

  static void insertFaultInjection(Module &M, Function &F, Instruction *I, int index, int f_index) {
    Constant *inject_func = getInjectFunc(M);
    Constant *profile_func = getProfileFunc(M);
    Instruction *alloca_insertPoint = I->getParent()->getParent()->begin()->getFirstNonPHIOrDbgOrLifetime();

    //
    // Inject Fault function
    //
    Type *return_type = I->getType();
    std::vector<Type*> inject_func_types(5);
    inject_func_types[0] = Type::getInt32Ty(M.getContext()); // f_index
    inject_func_types[1] = Type::getInt32Ty(M.getContext()); // index
    inject_func_types[2] = Type::getInt32Ty(M.getContext()); // dependecy
    inject_func_types[3] = return_type; // data
    inject_func_types[4] = PointerType::get(Type::getInt8Ty(M.getContext()), 0); // name
    ArrayRef<Type*> paramtypes_array_ref(inject_func_types);
    FunctionType* injectfunctype = FunctionType::get(return_type, paramtypes_array_ref, false);
    std::string funcname = getFIFuncNameforType(return_type);
    Constant *injectfunc = M.getOrInsertFunction(funcname, injectfunctype);
    std::vector<Value*> args(7);
    args[0] = ConstantInt::get(Type::getInt32Ty(M.getContext()), f_index);
    args[1] = ConstantInt::get(Type::getInt32Ty(M.getContext()), index);
    args[2] = ConstantInt::get(Type::getInt32Ty(M.getContext()), getDepedencyLevel(I));
    args[3] = I;
    std::string opcode_str = I->getOpcodeName();
    GlobalVariable* opcode_str_gv = findOrCreateGlobalNameString(M, opcode_str);
    std::vector<Constant*> indices_for_gep(2);
    indices_for_gep[0] = ConstantInt::get(Type::getInt32Ty(M.getContext()), 0);
    indices_for_gep[1] = ConstantInt::get(Type::getInt32Ty(M.getContext()), 0);
    ArrayRef<Constant*> indices_for_gep_array_ref(indices_for_gep);
    Constant* gep_expr = ConstantExpr::getGetElementPtr(opcode_str_gv->getType(), 
      opcode_str_gv, indices_for_gep_array_ref, true);
    args[4] = gep_expr; // opcode in string
    ArrayRef<Value*> args_array_ref(args);
    Instruction *insertptr = I;
    Instruction *ficall = CallInst::Create(injectfunc, args_array_ref, "fi", insertptr);

    std::list<User*> inst_uses;
    for (Value::user_iterator user_it = I->user_begin();
      user_it != I->user_end(); ++user_it) {
      User *user = *user_it;
      if (user != ficall)
        inst_uses.push_back(user);
    }
    for (std::list<User*>::iterator use_it = inst_uses.begin();
      use_it != inst_uses.end(); ++use_it) {
      User *user = *use_it;
      user->replaceUsesOfWith(I, ficall);
    }

    //
    // Profile function
    //
    //const char *opcodeName = I->getOpcodeName();
    //const std::string str(I->getOpcodeName());
    //ArrayRef<uint8_t> opcode_name_array_ref((uint8_t*)opcodeName, str.size() + 1);
    //llvm::Value* OPCodeName = llvm::ConstantDataArray::get(M.getContext(), opcode_name_array_ref);
    //AllocaInst* OPCodePtr = new AllocaInst(OPCodeName->getType(), 
    //  F.getParent()->getDataLayout().getProgramAddressSpace(), "llfi_trace", alloca_insertPoint);
    //new StoreInst(OPCodeName, OPCodePtr, insertPoint);
  }

private:

#pragma region Meta Functions

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

#pragma endregion

#pragma region Fault Injection Functions

  static std::string intToString(int i) {
    std::stringstream s;
    s << i;
    return s.str();
  }

  static std::map<const Type*, std::string> fi_rettype_funcname_map;
  static std::string getFIFuncNameforType(const Type *type) {
    std::string funcname;
    if (fi_rettype_funcname_map.find(type) != fi_rettype_funcname_map.end()) {
      funcname = fi_rettype_funcname_map[type];
    } else {
      funcname = "inject_fault";
      int ficount = fi_rettype_funcname_map.size();
      funcname += intToString(ficount);
      fi_rettype_funcname_map[type] = funcname;
    }
    return funcname;
  }

  static GlobalVariable* findOrCreateGlobalNameString(Module &M, std::string name)
  {
    LLVMContext& context = M.getContext();
    std::string str_suffix = std::string("_namestr");
    GlobalVariable* nameStr = M.getGlobalVariable(name + str_suffix, true);
    if (nameStr != NULL)
      return nameStr;
    std::string gv_nameStr = name + str_suffix;
    Constant* name_c = ConstantDataArray::getString(context, name);
    nameStr = new GlobalVariable(name_c->getType(), true, GlobalVariable::InternalLinkage, name_c, gv_nameStr.c_str());
    M.getGlobalList().push_back(nameStr);
    return nameStr;
  }

  static Constant *getProfileFunc(Module &M) {
    std::vector<Type*> profile_func_param_types(4);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);
    profile_func_param_types[1] = Type::getInt32Ty(context);
    profile_func_param_types[2] = PointerType::get(Type::getInt8Ty(context), 0);
    profile_func_param_types[3] = PointerType::get(Type::getInt8Ty(context), 0);

    ArrayRef<Type*> profile_func_param_types_array_ref(profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(Type::getVoidTy(context),
      profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject_profile", fi_init_func_type);
  }

  static Constant *getInjectFunc(Module &M) {
    std::vector<Type*> profile_func_param_types(4);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);
    profile_func_param_types[1] = Type::getInt32Ty(context);
    profile_func_param_types[2] = Type::getInt32Ty(context);
    profile_func_param_types[3] = PointerType::get(Type::getInt8Ty(context), 0);

    ArrayRef<Type*> profile_func_param_types_array_ref(profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(Type::getVoidTy(context),
      profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject", fi_init_func_type);
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

  static void createInjectionFunctions(Module &M) {
    Constant *pre_fi_func = getDetermineFunc(M);
    Constant *injectfunc = getInjectFunc(M);
    Constant *profilefunc = getProfileFunc(M);

    for (std::map<const Type*, std::string>::const_iterator fi =
      fi_rettype_funcname_map.begin();
      fi != fi_rettype_funcname_map.end(); ++fi) {
      const Type *fi_type = fi->first;

      Type *fi_type_unconst = const_cast<Type*>(fi_type);

      std::string fi_name = fi->second;
      createInjectionFuncforType(M, fi_type_unconst, fi_name, injectfunc, pre_fi_func);
    }
  }

  static void createInjectionFuncforType(Module &M, Type *fitype, std::string &fi_name,
    Constant *injectfunc, Constant *pre_fi_func) {
    LLVMContext &context = M.getContext();
    Function *f = M.getFunction(fi_name);
    std::vector<Value*> args;
    for (Function::arg_iterator ai = f->arg_begin(); ai != f->arg_end(); ++ai)
      args.push_back(&*ai);

    BasicBlock* entryblock = BasicBlock::Create(context, "entry", f);
    // store the value of target instruction to memory
    AllocaInst *tmploc = new AllocaInst(fitype, M.getDataLayout().getProgramAddressSpace(), "tmploc", entryblock);
    new StoreInst(args[1], tmploc, entryblock);

    std::vector<Value*> pre_fi_args(3);
    pre_fi_args[0] = args[0];
    pre_fi_args[1] = args[1];
    pre_fi_args[2] = args[2];
    ArrayRef<Value*> pre_fi_args_array_ref(pre_fi_args);
    Value *prefuncval = CallInst::Create(pre_fi_func, pre_fi_args_array_ref, "pre_cond", entryblock);

    BasicBlock *fiblock = BasicBlock::Create(context, "inject", f);
    BasicBlock *exitblock = BasicBlock::Create(context, "exit", f);
    //if prefuncval is true, goto inject function
    BranchInst::Create(fiblock, exitblock, prefuncval, entryblock);
    BranchInst *fi2exit_branch = BranchInst::Create(exitblock, fiblock);

    std::vector<Value*> fi_args(4);
    fi_args[0] = args[0]; //LLFI index
    int size = fitype->getScalarSizeInBits();
    fi_args[1] = args[1]; //size
    //fi_args[2] = new BitCastInst(tmploc,
    //  PointerType::get(Type::getInt8Ty(context), 0),
    //  "tmploc_cast", fi2exit_branch); //pointer to target memory
    fi_args[2] = ConstantInt::get(Type::getInt32Ty(context), size);
    fi_args[3] = args[3];
    //fi_args[4] = args[5];
    //fi_args[5] = args[6];
                         
    ArrayRef<Value*> fi_args_array_ref(fi_args);

    CallInst::Create(injectfunc, fi_args_array_ref, "",
      fi2exit_branch);

    LoadInst *updateval = new LoadInst(tmploc, "updateval", exitblock);
    ReturnInst::Create(context, updateval, exitblock);
  }

  static int getDepedencyLevel(Instruction *I) {
    auto mgr = getInfoManager(I->getDebugLoc()->getLine());
    if (mgr == nullptr)
      return -1;
    mgr->doFolding();
    return (*mgr->begin())->getType();
  }

#pragma endregion
};

///---------------------------------------------------------
///
///                   Profile Collector
///
///---------------------------------------------------------

class FaultInjectionTargetSelector {
  Function *target_function;
  std::vector<Instruction*> selected;

public:
  FaultInjectionTargetSelector(Function *TargetFunction)
    : target_function(TargetFunction) {
  }

  void selectInstructions() {

  }

  std::vector<Instruction*> getSelectedInsts() {
    return selected;
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

    int count_of_selection = 0;
    int f_index = 0;

    for (Module::iterator m_it = M.begin(); m_it != M.end(); ++m_it, ++f_index) {
      FaultInjectionTargetSelector selector(&*m_it);
      selector.selectInstructions();
      for (auto& inst : selector.getSelectedInsts()) {
        FaultInjectionInsertMachine::insertFaultInjection(M, *m_it, inst, count_of_selection, f_index);
        count_of_selection++;
      }
    }

    FaultInjectionInsertMachine::insertMetaFunc(M, count_of_selection);

    return true;
  }

};

} // namespace

char LLVMFaultInjectionPass::ID = 0;

static RegisterPass<LLVMFaultInjectionPass> X("faultinject", "Fault Injection Pass",
  false /* Only looks at CFG */,
  false /* Analysis Pass */);
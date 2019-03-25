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
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Dependency.h"
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <string>

#define DEBUG_TYPE "fault-injection"

#define MARK_FUNCTION_NAME "__marking_faultinject"
#define IGNORE_ZERO_SIZE 1
#define USE_RAW_INJECT 1

using namespace llvm;
static std::map<const Type *, std::string> fi_rettype_funcname_map;
namespace {

void genFullNameOpcodeMap(std::map<std::string, unsigned> &opcodenamemap) {
#define HANDLE_INST(N, OPC, CLASS) \
  opcodenamemap[std::string(Instruction::getOpcodeName(N))] = N;
#include "llvm/IR/Instruction.def"
}
static int a = 1;

///---------------------------------------------------------
///
///                    Insert Machine
///
///---------------------------------------------------------

class FaultInjectionInsertMachine {
 public:
  //
  // Insert code that does not depend on Instruction or Register.
  //
  static void insertMetaFunc(Module &M) {
    //
    // Find main function
    //
    Function *mainFunc = M.getFunction("main");
    BasicBlock *entryBB = &mainFunc->front();

    //
    // Insert 'fault_inject_init' function.
    //
    Constant *initFunc = getInitFunction(M);
    CallInst::Create(initFunc, "", entryBB->getFirstNonPHI());

    //
    // Insert 'fault_inject_finish' function.
    //
    Constant *finishFunc = getFinishFunction(M);

    std::set<Instruction *> exitInsts;
    getProgramExitInsts(M, exitInsts);
    assert(exitInsts.size() != 0 &&
           "Program does not have explicit exit point");
    for (std::set<Instruction *>::iterator it = exitInsts.begin();
         it != exitInsts.end(); ++it) {
      Instruction *term = *it;
      CallInst::Create(finishFunc, "", term);
    }

    //
    // Insert 'inject_fault#num' functions.
    //
    createInjectionFunctions(M);
  }

  static void insertFaultInjection(Module &M, Function &F, Instruction *I,
                                   int index, int f_index,
                                   std::vector<int> &regs) {
    for (auto reg : regs) insertFaultInjection(M, F, I, index, f_index, reg);
  }

  //
  // Inject 'inject_fault#num' functions for test fault-injection.
  //
  static bool insertFaultInjection(Module &M, Function &F, Instruction *I,
                                   int index, int f_index, int reg_num) {
    Constant *inject_func = getInjectFunc(M);
    Constant *trace_func = getTraceFunc(M);
    Instruction *alloca_insertPoint =
        I->getParent()->getParent()->begin()->getFirstNonPHIOrDbgOrLifetime();
    Value *target = I;

    if (reg_num != -1) {
      target = I->getOperand(reg_num);
    }

    //
    // Get 'inject_fault#num' parameter types.
    //
    Type *return_type = target->getType();
#if IGNORE_ZERO_SIZE
    if (return_type->getScalarSizeInBits() == 0) return false;
#endif
    std::vector<Type *> inject_func_types(6);
    inject_func_types[0] = Type::getInt32Ty(M.getContext());  // f_index
    inject_func_types[1] = Type::getInt32Ty(M.getContext());  // index
    inject_func_types[2] = Type::getInt32Ty(M.getContext());  // reg_num
    inject_func_types[3] = Type::getInt32Ty(M.getContext());  // dependecy
    inject_func_types[4] = return_type;                       // data
    inject_func_types[5] =
        PointerType::get(Type::getInt8Ty(M.getContext()), 0);  // name
    ArrayRef<Type *> paramtypes_array_ref(inject_func_types);
    FunctionType *injectfunctype =
        FunctionType::get(return_type, paramtypes_array_ref, false);
    std::string funcname = getFIFuncNameforType(return_type);
    Constant *injectfunc = M.getOrInsertFunction(funcname, injectfunctype);

    //
    // Set 'inject_fault#num' parameters.
    //
    std::vector<Value *> args(6);
    args[0] = ConstantInt::get(Type::getInt32Ty(M.getContext()), f_index);
    args[1] = ConstantInt::get(Type::getInt32Ty(M.getContext()), index);
    args[2] = ConstantInt::get(Type::getInt32Ty(M.getContext()), reg_num);
    args[3] = ConstantInt::get(Type::getInt32Ty(M.getContext()),
                               getDepedencyLevel(I));
    args[4] = target;
    std::string opcode_str = I->getOpcodeName();
    GlobalVariable *opcode_str_gv = findOrCreateGlobalNameString(M, opcode_str);
    std::vector<Constant *> indices_for_gep(2);
    indices_for_gep[0] = ConstantInt::get(Type::getInt32Ty(M.getContext()), 0);
    indices_for_gep[1] = ConstantInt::get(Type::getInt32Ty(M.getContext()), 0);
    ArrayRef<Constant *> indices_for_gep_array_ref(indices_for_gep);
    Constant *gep_expr = ConstantExpr::getGetElementPtr(
        NULL, opcode_str_gv, indices_for_gep_array_ref, true);
    args[5] = gep_expr;  // opcode in
    ArrayRef<Value *> args_array_ref(args);
    Instruction *insertptr = getInsertPtrforRegsofInst(target, I);
    Instruction *ficall =
        CallInst::Create(injectfunc, args_array_ref, "fi", insertptr);

    //
    // Concatenates the results of an existing register with the
    // 'inject_fault#num' function.
    //
    std::list<User *> inst_uses;
    for (Value::user_iterator user_it = target->user_begin();
         user_it != target->user_end(); ++user_it) {
      User *user = *user_it;
      if (user != ficall) inst_uses.push_back(user);
    }
    for (std::list<User *>::iterator use_it = inst_uses.begin();
         use_it != inst_uses.end(); ++use_it) {
      User *user = *use_it;
      user->replaceUsesOfWith(target, ficall);

      if (isa<TerminatorInst>(user)) continue;

      //
      // Insert 'fault_inject_trace' function.
      //
      AllocaInst *tmploc = new AllocaInst(
          return_type, M.getDataLayout().getProgramAddressSpace(),
          "tmploc_" + intToString(a), &*F.getEntryBlock().begin());
      StoreInst *store_inst =
          new StoreInst(ficall, tmploc, cast<Instruction>(user)->getNextNode());
      std::vector<Value *> trace_args(7);
      trace_args[0] =
          ConstantInt::get(Type::getInt32Ty(M.getContext()), f_index);
      trace_args[1] = ConstantInt::get(Type::getInt32Ty(M.getContext()), index);
      trace_args[2] =
          ConstantInt::get(Type::getInt32Ty(M.getContext()), reg_num);
      trace_args[3] = ConstantInt::get(Type::getInt32Ty(M.getContext()),
                                       getDepedencyLevel(I));
      trace_args[4] =
          ConstantInt::get(Type::getInt32Ty(M.getContext()),
                           target->getType()->getScalarSizeInBits());  // size
      trace_args[5] = new BitCastInst(
          tmploc, PointerType::get(Type::getInt8Ty(M.getContext()), 0),
          "tmploc_cast_" + intToString(a++),
          cast<Instruction>(user)->getNextNode()->getNextNode());
      trace_args[6] = gep_expr;
      ArrayRef<Value *> trace_args_array_ref(trace_args);
      CallInst::Create(
          getTraceFunc(M), trace_args_array_ref, "",
          cast<Instruction>(user)->getNextNode()->getNextNode()->getNextNode());
    }

    return true;
  }

  static void insertRawFaultInjection(Module &M, Function &F, Instruction *I, std::mt19937& R) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 31);
    int loc = dist(R);

    auto xor_op = BinaryOperator::CreateXor(I, ConstantInt::get(Type::getInt32Ty(M.getContext()), ~(1 << loc)), "rfi", I->getNextNode());

    std::list<User *> inst_uses;
    for (Value::user_iterator user_it = I->user_begin();
      user_it != I->user_end(); ++user_it) {
      User *user = *user_it;
      if (user != I && user != xor_op) inst_uses.push_back(user);
    }
    for (std::list<User *>::iterator use_it = inst_uses.begin();
      use_it != inst_uses.end(); ++use_it) {
      User *user = *use_it;
      user->replaceUsesOfWith(I, xor_op);
    }

    F.print(errs());
  }

  static void insertDetermineLogicForRawFaultInjection(Module &M, Function &F, Instruction *I, Instruction *T, std::mt19937& R) {
    // Create XOR setter and xor operand resetter

    // ENTRY:
    // %xor_marker = alloca i32, align 4
    // store i32 (0~31 random number), %xor_marker, align 4
    // CORE:
    // %t = xor %target, %xor_marker
    // store i32 0, %xor_marker

    // pick random bits
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 31);
    int loc = dist(R);

    IntegerType *type = Type::getInt32Ty(M.getContext());
    AllocaInst *xor_marker =
      new AllocaInst(type, M.getDataLayout().getProgramAddressSpace(),
        "xor_marker", &*F.getEntryBlock().begin());
    Value *num = ConstantInt::get(type, loc, true);
    new StoreInst(num, xor_marker, &*++F.getEntryBlock().begin());

    // Create xor and resetter
    Value *num_zero = ConstantInt::get(type, 0, true);
    StoreInst *resetter = new StoreInst(num_zero, xor_marker, I);
    LoadInst *val = new LoadInst(xor_marker, "xor_val", resetter);
    BinaryOperator *fi = BinaryOperator::CreateXor(T, val, "rfi", val->getNextNode());

    for (int i = 0; i < I->getNumOperands(); i++)
      if (I->getOperand(i) == T)
        I->setOperand(i, fi);

    F.print(errs());
  }

 private:
#pragma region Meta Functions

  static Constant *getInitFunction(Module &M) {
    LLVMContext &context = M.getContext();

    FunctionType *fi_init_func_type =
        FunctionType::get(Type::getVoidTy(context), false);
    return M.getOrInsertFunction("fault_inject_init", fi_init_func_type);
  }

  static Constant *getFinishFunction(Module &M) {
    LLVMContext &context = M.getContext();

    FunctionType *fi_init_func_type =
        FunctionType::get(Type::getVoidTy(context), false);
    return M.getOrInsertFunction("fault_inject_finish", fi_init_func_type);
  }

  static Instruction *getTermInstofFunction(Function *func) {
    BasicBlock &termbb = func->back();
    Instruction *ret = termbb.getTerminator();

    assert(isa<ReturnInst>(ret) ||
           isa<UnreachableInst>(ret) &&
               "Last instruction is not return or exit() instruction");
    return ret;
  }

  static void getProgramExitInsts(Module &M,
                                  std::set<Instruction *> &exitinsts) {
    for (Module::iterator m_it = M.begin(); m_it != M.end(); ++m_it) {
      if (!m_it->isDeclaration()) {
        for (inst_iterator f_it = inst_begin(*m_it); f_it != inst_end(*m_it);
             ++f_it) {
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

    Function *mainfunc = M.getFunction("main");
    exitinsts.insert(getTermInstofFunction(mainfunc));
  }

#pragma endregion

#pragma region Fault Injection Functions

  static Instruction *getInsertPtrforRegsofInst(Value *reg, Instruction *inst) {
    if (reg == inst) {
      if (inst->isTerminator()) {
        errs() << "ERROR: LLFI not able to inject into destination register of "
               << *inst << ", change isRegofInstInjectable() to fix it\n";
        exit(2);
      } else {
        Instruction *nn = inst->getNextNode();
        while (isa<PHINode>(nn)) nn = nn->getNextNode();
        return &*nn;
      }
    } else {
      if (isa<PHINode>(inst)) {
        errs() << "ERROR: LLFI not able to inject into source register of "
               << *inst << ", change isRegofInstInjectable to fix it\n";
        exit(2);
      }
      return inst->getNextNode();
    }
  }

  static std::string intToString(int i) {
    std::stringstream s;
    s << i;
    return s.str();
  }

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

  static GlobalVariable *findOrCreateGlobalNameString(Module &M,
                                                      std::string name) {
    LLVMContext &context = M.getContext();
    std::string str_prefix = std::string("fault_inject_");
    std::string str_suffix = std::string("_namestr");
    GlobalVariable *nameStr =
        M.getGlobalVariable(str_prefix + name + str_suffix, true);
    if (nameStr != NULL) return nameStr;
    std::string gv_nameStr = str_prefix + name + str_suffix;
    Constant *name_c = ConstantDataArray::getString(context, name);
    nameStr = new GlobalVariable(name_c->getType(), true,
                                 GlobalVariable::InternalLinkage, name_c,
                                 gv_nameStr.c_str());
    M.getGlobalList().push_back(nameStr);
    return nameStr;
  }

  static Constant *getTraceFunc(Module &M) {
    std::vector<Type *> profile_func_param_types(7);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);  // function index
    profile_func_param_types[1] = Type::getInt32Ty(context);  // index
    profile_func_param_types[2] = Type::getInt32Ty(context);  // register number
    profile_func_param_types[3] = Type::getInt32Ty(context);  // dependency
    profile_func_param_types[4] = Type::getInt32Ty(context);  // size
    profile_func_param_types[5] =
        PointerType::get(Type::getInt8Ty(context), 0);  // value
    profile_func_param_types[6] =
        PointerType::get(Type::getInt8Ty(context), 0);  // opcode name

    ArrayRef<Type *> profile_func_param_types_array_ref(
        profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(
        Type::getVoidTy(context), profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject_trace", fi_init_func_type);
  }

  static Constant *getInjectFunc(Module &M) {
    std::vector<Type *> profile_func_param_types(7);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);  // function index
    profile_func_param_types[1] = Type::getInt32Ty(context);  // index
    profile_func_param_types[2] = Type::getInt32Ty(context);  // register number
    profile_func_param_types[3] = Type::getInt32Ty(context);  // dependency
    profile_func_param_types[4] = Type::getInt32Ty(context);  // size
    profile_func_param_types[5] =
        PointerType::get(Type::getInt8Ty(context), 0);  // value
    profile_func_param_types[6] =
        PointerType::get(Type::getInt8Ty(context), 0);  // opcode name

    ArrayRef<Type *> profile_func_param_types_array_ref(
        profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(
        Type::getVoidTy(context), profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject", fi_init_func_type);
  }

  static Constant *getDetermineFunc(Module &M) {
    std::vector<Type *> profile_func_param_types(4);
    LLVMContext &context = M.getContext();
    profile_func_param_types[0] = Type::getInt32Ty(context);  // function index
    profile_func_param_types[1] = Type::getInt32Ty(context);  // index
    profile_func_param_types[2] = Type::getInt32Ty(context);  // register number
    profile_func_param_types[3] = Type::getInt32Ty(context);  // dependency

    ArrayRef<Type *> profile_func_param_types_array_ref(
        profile_func_param_types);

    FunctionType *fi_init_func_type = FunctionType::get(
        Type::getInt1Ty(context), profile_func_param_types_array_ref, false);
    return M.getOrInsertFunction("fault_inject_determine", fi_init_func_type);
  }

  static void createInjectionFunctions(Module &M) {
    Constant *pre_fi_func = getDetermineFunc(M);
    Constant *injectfunc = getInjectFunc(M);
    Constant *tracefunc = getTraceFunc(M);

    for (std::map<const Type *, std::string>::const_iterator fi =
             fi_rettype_funcname_map.begin();
         fi != fi_rettype_funcname_map.end(); ++fi) {
      const Type *fi_type = fi->first;

      Type *fi_type_unconst = const_cast<Type *>(fi_type);

      std::string fi_name = fi->second;
      createInjectionFuncforType(M, fi_type_unconst, fi_name, injectfunc,
                                 pre_fi_func);
    }
  }

  static void createInjectionFuncforType(Module &M, Type *fitype,
                                         std::string &fi_name,
                                         Constant *injectfunc,
                                         Constant *pre_fi_func) {
    LLVMContext &context = M.getContext();
    Function *f = M.getFunction(fi_name);
    std::vector<Value *> args;
    for (Function::arg_iterator ai = f->arg_begin(); ai != f->arg_end(); ++ai)
      args.push_back(&*ai);

    BasicBlock *entryblock = BasicBlock::Create(context, "entry", f);
    // store the value of target instruction to memory
    AllocaInst *tmploc =
        new AllocaInst(fitype, M.getDataLayout().getProgramAddressSpace(),
                       "tmploc", entryblock);
    new StoreInst(args[4], tmploc, entryblock);

    std::vector<Value *> pre_fi_args(4);
    pre_fi_args[0] = args[0];  // f_index
    pre_fi_args[1] = args[1];  // index
    pre_fi_args[2] = args[2];  // reg_num
    pre_fi_args[3] = args[3];  // dependency
    ArrayRef<Value *> pre_fi_args_array_ref(pre_fi_args);
    Value *prefuncval = CallInst::Create(pre_fi_func, pre_fi_args_array_ref,
                                         "pre_cond", entryblock);

    BasicBlock *fiblock = BasicBlock::Create(context, "inject", f);
    BasicBlock *exitblock = BasicBlock::Create(context, "exit", f);
    // if prefuncval is true, goto inject function
    BranchInst::Create(fiblock, exitblock, prefuncval, entryblock);
    BranchInst *fi2exit_branch = BranchInst::Create(exitblock, fiblock);

    std::vector<Value *> fi_args(7);
    fi_args[0] = args[0];  // f_index
    int size = fitype->getScalarSizeInBits();
    fi_args[1] = args[1];  // index
    fi_args[2] = args[2];  // reg_num
    fi_args[3] = args[3];  // dependency
    fi_args[4] = ConstantInt::get(Type::getInt32Ty(context), size);  // size
    fi_args[5] = new BitCastInst(
        tmploc, PointerType::get(Type::getInt8Ty(context), 0), "tmploc_cast",
        fi2exit_branch);   // pointer to target memory
    fi_args[6] = args[5];  // opcode name

    ArrayRef<Value *> fi_args_array_ref(fi_args);

    CallInst::Create(injectfunc, fi_args_array_ref, "", fi2exit_branch);

    LoadInst *updateval = new LoadInst(tmploc, "updateval", exitblock);
    ReturnInst::Create(context, updateval, exitblock);
  }

  static int getDepedencyLevel(Instruction *I) {
    // auto mgr = getInfoManager(I->getDebugLoc()->getLine());
    // if (mgr == nullptr)
    return -1;
    // mgr->doFolding();
    // return (*mgr->begin())->getType();
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
  std::vector<std::pair<Instruction *, int>> selected;

 public:
  FaultInjectionTargetSelector(Function *TargetFunction)
      : target_function(TargetFunction) {}

  void selectInstructions() {
    for (auto &bb : *target_function) {
      for (auto &inst : bb) {
#if !USE_RAW_INJECT
        // [===== Store Instruction =====]
        // if (isa<StoreInst> (inst)) {
        //  if (inst.getOperand(0)->getType()->isIntegerTy())
        //    continue;

        //  selected.push_back({ &inst, 0 });
        //  inst.print(errs(), true);
        //  errs() << "\ninstrcution selected\n";
        //}

        //// [===== Load Instruction =====]
        // else if (isa<LoadInst>(inst)) {
        //  if (!isa<Instruction>(inst.getOperand(0)))
        //    continue;
        //
        //  selected.push_back({ &inst, -1 });
        //  inst.print(errs(), true);
        //  errs() << "\ninstrcution selected\n";
        //}

        // destination register
        if (isRegofInstFITarget(&inst, &inst))
          if (isRegofInstInjectable(&inst, &inst))
            selected.push_back({&inst, -1});

        // source register
        int pos = 0;
        for (User::op_iterator op_it = inst.op_begin(); op_it != inst.op_end();
             ++op_it, ++pos) {
          Value *src = *op_it;
          if (isRegofInstFITarget(src, &inst, pos)) {
            if (isRegofInstInjectable(src, &inst)) {
              selected.push_back({&inst, pos});
            } else {
              errs() << "LLFI cannot inject faults in source reg ";
              if (isa<BasicBlock>(src))
                errs() << src->getName();
              else
                errs() << *src;
              errs() << " of instruction " << inst << "\n";
            }
          }
        }

        /*if (isa<StoreInst>(inst))
          selected.push_back({ &inst, 1 });*/
#else
        if (CallInst *call = dyn_cast<CallInst>(&inst)) {
          if (call->getCalledFunction()->getName().startswith(MARK_FUNCTION_NAME)) {
            auto target = cast<Instruction>(call->getOperand(0));
            selected.push_back({ target, 0 });
            std::list<User *> inst_uses;
            for (Value::user_iterator user_it = target->user_begin();
              user_it != target->user_end(); ++user_it) {
              User *user = *user_it;
              if (user != call) inst_uses.push_back(user);
            }
            for (std::list<User *>::iterator use_it = inst_uses.begin();
              use_it != inst_uses.end(); ++use_it) {
              if (!isa<PHINode>(*use_it)) {
                selected.push_back({ cast<Instruction>(*use_it),-1 });
                errs() << "SELECT : " << *cast<Instruction>(*use_it) << '\n';
              }
            }
          }
        }
#endif
      }
    }
  }

  std::vector<std::pair<Instruction *, int>> getSelectedInsts() {
    return selected;
  }

 private:
  bool isRegofInstFITarget(Value *reg, Instruction *inst) { return true; }

  bool isRegofInstFITarget(Value *reg, Instruction *inst, int pos) {
    return false;
  }

  bool isRegofInstInjectable(Value *reg, Instruction *inst) {
    // TODO: keep updating
    // if we find anything that can be covered, remove them from the checks
    // if we find new cases that we cannot handle, add them to the checks
    if (reg == inst) {
      if (inst->getType()->isVoidTy() || isa<TerminatorInst>(inst)) {
        return false;
      }
    } else {
      if (isa<BasicBlock>(reg) || isa<PHINode>(inst)) return false;
    }
    return true;
  }
};

class ProfileCollector {
  std::set<std::string> function_name;

 public:
  static ProfileCollector *Instance;

  ProfileCollector(Module &M) { getModuleFuncs(M); }

  static ProfileCollector *getInstance(Module &M) {
    if (Instance == nullptr) Instance = new ProfileCollector(M);
    return Instance;
  }

  void getModuleFuncs(Module &M) {
    for (Module::iterator it = M.begin(); it != M.end(); ++it) {
      std::string func_name = it->getName().str();
      // std::string final_name = demangleFuncName(func_name);

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

  LLVMFaultInjectionPass() : ModulePass(ID) {}

  ~LLVMFaultInjectionPass() {}

  bool runOnModule(Module &M) override {
    if (!M.getFunction("main")) {
      errs() << "Error: main function not found.";
      exit(1);
    }

    int count_of_selection = 0;
    int f_index = 0;

    std::mt19937 rng;
    rng.seed(std::random_device()());

    for (Module::iterator m_it = M.begin(); m_it != M.end();
         ++m_it, ++f_index) {
      if (m_it->getName() != "main") continue;
      FaultInjectionTargetSelector selector(&*m_it);
      selector.selectInstructions();
#if !USE_RAW_INJECT
      for (auto &inst : selector.getSelectedInsts()) {
        if (FaultInjectionInsertMachine::insertFaultInjection(
          M, *m_it, inst.first, count_of_selection, f_index, inst.second))
          count_of_selection++;
      }
#else
      if (selector.getSelectedInsts().size() == 0) continue;
      std::uniform_int_distribution<std::mt19937::result_type> dist(1, selector.getSelectedInsts().size() - 1);
      auto selected = selector.getSelectedInsts()[dist(rng)].first;

      FaultInjectionInsertMachine::insertDetermineLogicForRawFaultInjection(M, *m_it, selected, selector.getSelectedInsts()[0].first, rng);
      break;
#endif
    }

#if !USE_RAW_INJECT
    FaultInjectionInsertMachine::insertMetaFunc(M);
#endif

    return true;
  }
};

}  // namespace

char LLVMFaultInjectionPass::ID = 0;

static RegisterPass<LLVMFaultInjectionPass> X("faultinject",
                                              "Fault Injection Pass",
                                              false /* Only looks at CFG */,
                                              false /* Analysis Pass */);
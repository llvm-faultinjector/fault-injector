# Fault Injection Tool for Hardware Injection

This project is compact version of llfi. (https://github.com/DependableSystemsLab/LLFI)

## Requirements

```
LLVM 7.0.0 (or Later)
```

## Build

All you need is an appropriately built 'opt' to use our tools.

```
Copy 'FaultInjector.cpp' to 'llvm/tools/opt'.
Insert 'FaultInjector.cpp' in 'llvm/tools/opt/CMakeLists.txt:add_llvm_tool.
Make directory 'llvm/build'
Move 'llvm/build'
Build your opt using cmake.

You can also use loadable library.
```

## Test

```
Make test folder under 'llvm/build'.
Copy 'rtl-core.c' to 'llvm/build/test'.
Write your own test file (or our a.c).
Then run this command lines.

--------

clang -S -emit-llvm a.c
./../opt -faultinject a.ll -S -o b.ll
llc -filetype=obj b.ll -o b.obj

clang -S -emit-llvm rtl-core.c
llc -filetype=obj rtl-core.ll -o rtl-core.obj

clang rtl-core.obj b.obj
```

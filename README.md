# Fault Injector for Hardware Injection

This project is compact version of llfi. (https://github.com/DependableSystemsLab/LLFI)

## Dependency

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

## Example a.c profile

``` c
#include <stdio.h>

int main(int argc, char *argv[])
{
  int a = 0, b = 0;
  for (int i = 0; i < argc; i++) {
    b += i;
  }

  printf("%d", b);
}
```

```
[Fault Injector] RTL-Core Init!
[Fault Injector] f_index=04, index=00, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=01, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=02, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=03, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=02, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=07, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=10, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] f_index=04, index=09, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000000
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=12, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] f_index=04, index=13, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=02, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=07, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=10, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] f_index=04, index=09, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=12, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] f_index=04, index=13, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000002
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=02, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=07, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=10, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] f_index=04, index=09, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=12, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] f_index=04, index=13, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=02, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=07, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=10, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] f_index=04, index=09, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000006
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=12, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] f_index=04, index=13, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000004
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=02, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=07, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000004
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=10, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] f_index=04, index=09, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000004
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=0000000A
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=12, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000004
[Fault Injector] f_index=04, index=13, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000005
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=05, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=02, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=07, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] f_index=04, index=04, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] f_index=04, index=14, reg_num=-1, dependency=-1, opcode=   load, size=32, value=0000000A
[Fault Injector] f_index=04, index=00, reg_num=-1, dependency=-1, opcode= alloca, size=00, value=
[Fault Injector] RTL-Core Finish!
```

Follow index 11th.

```
[Fault Injector] RTL-Core Init!
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000000
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000006
[Fault Injector] f_index=04, index=11, reg_num=-1, dependency=-1, opcode=    add, size=32, value=0000000A
[Fault Injector] RTL-Core Finish!
```

## Example a.c Fault Inject

```
```

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

If you use a command line such as `./a 1 1 1 1 1`, you will get the following outputs.

```
10
```

10 = 0 + 1 + 2 + 3 + 4

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

## How to work?

### 1. LLVM Fault Injection Pass

The role of `LLVM Fault Injection Pass` is to find `instructions` or `register` to 
inject Fault and to link with `runtime library core`. Since the actual linking is 
done in the `linker`, so the major role is to create the appropriate `object file`.

#### 1.1. Inject fault-injection code

##### 1.1.1. Select instructions and registers

##### 1.1.2. Register bypassing and inject 'inject_fault#num' function

##### 1.1.3. Insert 'fault_inject_trace' function

#### 1.2. Insert meta functions

##### 1.2.1. Insert 'fault_inject_init' function

##### 1.2.2. Insert 'fault_inject_finish' function

##### 1.2.3. Insert 'inject_fault#num' function

### 2. Runtime Library Core

The runtime library contains the core code that performs the actual injection operation 
and the log function that records various changes.

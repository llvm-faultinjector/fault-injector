# Fault Injector for Hardware Injection

This project is compact version of llfi. (https://github.com/DependableSystemsLab/LLFI)

LLFI is too old to work with the latest version of LLVM. So we have created a 
simple Fault Injection Tool that can run on the latest version of LLVM.

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
Ref: https://llvm.org/docs/WritingAnLLVMPass.html#quick-start-writing-hello-world
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
15
```

15 = 0 + 1 + 2 + 3 + 4 + 5

```
[Fault Injector] RTL-Core Init!
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000004
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000004
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000004
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=0000000A
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000004
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000005
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=0000000A
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=0000000F
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000005
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=08, reg_num=-1, dependency=-1, opcode=   load, size=32, value=0000000F
[Fault Injector] RTL-Core Finish!
[Fault Injector] determine=54
```

Follow index 05th.

```
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=0000000A
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=0000000F
```

## Example a.c Fault Inject

`setting` file.

```
run_fault_injection 1
count_of_determine 54
```

`result` file.

```
[Fault Injector] RTL-Core Init!
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000000
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000001
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000002
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=04, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=03, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=05, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=06, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000003
[Fault Injector] trace: f_index=04, index=07, reg_num=-1, dependency=-1, opcode=    add, size=32, value=00000004
[Fault Injector] inject_before: f_index=04, index=02, reg_num=-1, dependency=-1, opcode=   icmp, size=01, value=1
[Fault Injector]  inject_after: f_index=04, index=02, reg_num=-1, dependency=-1, opcode=   icmp, size=01, value=0
[Fault Injector] trace: f_index=04, index=01, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] trace: f_index=04, index=00, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000004
[Fault Injector] trace: f_index=04, index=08, reg_num=-1, dependency=-1, opcode=   load, size=32, value=00000006
[Fault Injector] RTL-Core Finish!
[Fault Injector] determine=38
```

When `i (reg_num=07)` was 4 , the control flow changed, and loop escaped.

```
6
```

## insertDetermineLogicForRawFaultInjection

Before injection

``` llvm
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %3 = icmp sgt i32 %0, 0
  br i1 %3, label %7, label %4

; <label>:4:                                      ; preds = %7, %2
  %5 = phi i32 [ 0, %2 ], [ %10, %7 ]
  %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"??_C@_02DPKJAMEF@?$CFd?$AA@", i64 0, i64 0), i32 %5)
  ret i32 0

; <label>:7:                                      ; preds = %2, %7
  %8 = phi i32 [ %11, %7 ], [ 0, %2 ]
  %9 = phi i32 [ %10, %7 ], [ 0, %2 ]
  tail call void @__marking_faultinject_int(i32 %9) #3
  %10 = add nuw nsw i32 %8, %9
  %11 = add nuw nsw i32 %8, 1
  %12 = icmp eq i32 %11, %0
  br i1 %12, label %4, label %7
}
```

After injection

``` llvm
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %xor_marker = alloca i32
  store i32 4, i32* %xor_marker
  %3 = icmp sgt i32 %0, 0
  br i1 %3, label %7, label %4

; <label>:4:                                      ; preds = %7, %2
  %5 = phi i32 [ 0, %2 ], [ %10, %7 ]
  %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"??_C@_02DPKJAMEF@?$CFd?$AA@", i64 0, i64 0), i32 %5)
  ret i32 0

; <label>:7:                                      ; preds = %7, %2
  %8 = phi i32 [ %11, %7 ], [ 0, %2 ]
  %9 = phi i32 [ %10, %7 ], [ 0, %2 ]
  tail call void @__marking_faultinject_int(i32 %9) #3
  %xor_val = load i32, i32* %xor_marker
  %rfi = xor i32 %9, %xor_val
  store i32 0, i32* %xor_marker
  %10 = add nuw nsw i32 %8, %rfi
  %11 = add nuw nsw i32 %8, 1
  %12 = icmp eq i32 %11, %0
  br i1 %12, label %4, label %7
}
```

## How to work?

### 0. Why are Pass and RTL separated?

It is very inefficient to provide a customized program based on user needs, 
so we provide `RTL` for profiling directly from the program.

### 1. LLVM Fault Injection Pass

The role of `LLVM Fault Injection Pass` is to find `instructions` or `register` to 
inject Fault and to link with `runtime library core`. Since the actual linking is 
done in the `linker`, the major role is to create the appropriate `object file`.

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

## Run Options (RTL Options)

### Select instruction

### Select register

### Select function

### Select variable (require dependency-check pass)

About dependency-check pass, go to https://github.com/rollrat/runnable-llvm.

### Select basic block

### Control fault probability

#### Set zero probability for profile

#### Set drawing lots probability

#### Set an unconditional single injection per running

//===----------------------------------------------------------------------===//
//
//                       LLVM Fault Injection Tool
//
//                         Runtime Library Core
//
//===----------------------------------------------------------------------===//
//
//  Copyright (C) 2019. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INDEX 65535
#define PRINT_MESSAGE 1

#define SETTING_FILE_NAME     "setting"
#define RESULT_FILE_NAME      "result"

typedef uint32_t major_type;

// profile option
// 0: profile, 1: fault-inject
static int run_fault_injection = 0;

// fault option
static int injection_type = 0;
static int count_of_determine = 54;

// print option
static int trace_function = -1;
static int trace_index = -1;
static int trace_dependency = -1;
static int trace_register = -2;

// profile
static int determine_count = 0;

void fault_inject_init() {
#if PRINT_MESSAGE
  printf("[Fault Injector] RTL-Core Init!\n");
#endif

  srand(time(NULL));

  //
  // TODO: Read setting file.
  //

  //
  // TODO: Initialize vars.
  //

  if (run_fault_injection == 0) return;

}

static void print_log(uint32_t f_index, uint32_t index, uint32_t reg_num,
  uint32_t dependency, uint32_t size, char *value,
  char *opcode, int simple, const char *str) {

  printf("[Fault Injector] %s: f_index=%02d, index=%02d, reg_num=%02d, dependency=%d",
      str, f_index, index, reg_num, dependency);

  if (simple == 0) {
    printf(", opcode=%7s, size=%02d, value=", opcode, size);

    if (size == 1) {
      printf("%d", *value);
    } else {
      int sz = size / 8;
      for (int i = 0; i < sz; i++) printf("%02X", value[sz - i - 1] & 0xFF);
    }
  }
  printf("\n");
}

void fault_inject_trace(uint32_t f_index, uint32_t index, uint32_t reg_num,
                        uint32_t dependency, uint32_t size, char *value,
                        char *opcode) {
  if (trace_function >= 0 && f_index != trace_function) return;
  if (trace_index >= 0 && index != trace_index) return;
  if (trace_dependency >= 0 && dependency != trace_dependency) return;
  if (trace_register >= -1 && reg_num != trace_register) return;
  print_log(f_index, index, reg_num, dependency, size, value, opcode, 0, "trace");
}

#pragma region Inject Determine

static int determine(double rate) { 
  return (double)rand() / RAND_MAX < rate; 
}

major_type fault_inject_determine(major_type f_index, major_type index,
                                  major_type reg_num, major_type dependency) {
  determine_count += 1;
  if (run_fault_injection == 0) return 0;
  return determine((double)1 / count_of_determine);
}

#pragma endregion

#pragma region Fault Inject

static int get_fault_inject_pos(int sz) {
  return rand() % sz;
}

static int get_fault_inject_bit() {
  return rand() % (sizeof(char) * 8);
}

void fault_inject_flipbit(char *value) {
  *value ^= (1 << get_fault_inject_bit());
}

void fault_inject_set0(char *value) {
  *value &= ~(1 << get_fault_inject_bit());
}

void fault_inject_set1(char *value) { *value |= (1 << get_fault_inject_bit()); }

void fault_inject(uint32_t f_index, uint32_t index, uint32_t reg_num,
                  uint32_t dependency, uint32_t size, char *value,
                  char *opcode) {
  print_log(f_index, index, reg_num, dependency, size, value, opcode, 0, "inject_before");
  if (size == 1) *value ^= 1;
  else {
    int pos = get_fault_inject_pos(size / 8);
    switch (injection_type) {
      case 0:
        fault_inject_flipbit(&value[pos]);
        break;
      case 1:
        fault_inject_set0(&value[pos]);
        break;
      case 2:
        fault_inject_set1(&value[pos]);
        break;
    }
  }
  print_log(f_index, index, reg_num, dependency, size, value, opcode, 0, " inject_after");
}

#pragma endregion

void fault_inject_finish() {
#if PRINT_MESSAGE
  printf("[Fault Injector] RTL-Core Finish!\n");
#endif

  if (run_fault_injection != 0)
    printf("[Fault Injector] determine=%d\n", determine_count);

  //
  // TODO: Save result file.
  //
}
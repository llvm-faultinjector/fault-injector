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
#include <string.h>
#include <time.h>

#define MAX_INDEX 65535
#define PRINT_MESSAGE 1

#define SETTING_FILE_NAME "setting"
#define RESULT_FILE_NAME "result"

typedef uint32_t major_type;

// profile option
// 0: profile, 1: fault-inject
static int run_fault_injection = 0;

// fault option
static int injection_type = 0;
static int count_of_determine = 0;

// print option
static int trace_function = -1;
static int trace_index = -1;
static int trace_dependency = -1;
static int trace_register = -2;

// profile
static int determine_count = 0;

static FILE *log_file;

void fault_inject_init() {
  // Open write file.
  log_file = fopen(RESULT_FILE_NAME, "w");

#if PRINT_MESSAGE
  fprintf(log_file, "[Fault Injector] RTL-Core Init!\n");
#endif

  srand(time(NULL));

  // Read setting file.
  {
    FILE *fp;
    char op_name[255];
    int op_option = 0;

    if ((fp = fopen(SETTING_FILE_NAME, "r")) == NULL) {
      fprintf(log_file, "[Fault Injector] Setting file not found.\n");
      exit(1);
    }

    while (fscanf(fp, "%s %d", op_name, &op_option) != EOF) {
#define DECALE_OPTION(name)      \
  if (!strcmp(op_name, #name)) { \
    name = op_option;            \
    continue;                    \
  }
      DECALE_OPTION(run_fault_injection)

      DECALE_OPTION(injection_type)
      DECALE_OPTION(count_of_determine)

      DECALE_OPTION(trace_function)
      DECALE_OPTION(trace_index)
      DECALE_OPTION(trace_dependency)
      DECALE_OPTION(trace_register)
    }

    fclose(fp);
  }

  //
  // TODO: Initialize vars.
  //

  if (run_fault_injection == 0) return;
}

static void print_log(uint32_t f_index, uint32_t index, uint32_t reg_num,
                      uint32_t dependency, uint32_t size, char *value,
                      char *opcode, int simple, const char *str) {
  fprintf(log_file,
          "[Fault Injector] %s: f_index=%02d, index=%02d, reg_num=%02d, "
          "dependency=%d",
          str, f_index, index, reg_num, dependency);

  if (simple == 0) {
    fprintf(log_file, ", opcode=%7s, size=%02d, value=", opcode, size);

    if (size == 1) {
      fprintf(log_file, "%d", *value);
    } else {
      int sz = size / 8;
      for (int i = 0; i < sz; i++)
        fprintf(log_file, "%02X", value[sz - i - 1] & 0xFF);
    }
  }
  fprintf(log_file, "\n");
}

void fault_inject_trace(uint32_t f_index, uint32_t index, uint32_t reg_num,
                        uint32_t dependency, uint32_t size, char *value,
                        char *opcode) {
  if (trace_function >= 0 && f_index != trace_function) return;
  if (trace_index >= 0 && index != trace_index) return;
  if (trace_dependency >= 0 && dependency != trace_dependency) return;
  if (trace_register >= -1 && reg_num != trace_register) return;
  print_log(f_index, index, reg_num, dependency, size, value, opcode, 0,
            "trace");
}

#pragma region Inject Determine

static int determine(double rate) { return (double)rand() / RAND_MAX < rate; }

major_type fault_inject_determine(major_type f_index, major_type index,
                                  major_type reg_num, major_type dependency) {
  determine_count += 1;
  if (run_fault_injection == 0) return 0;
  return determine((double)1 / count_of_determine);
}

#pragma endregion

#pragma region Fault Inject

static int get_fault_inject_pos(int sz) { return rand() % sz; }

static int get_fault_inject_bit() { return rand() % (sizeof(char) * 8); }

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
  print_log(f_index, index, reg_num, dependency, size, value, opcode, 0,
            "inject_before");
  if (size == 1)
    *value ^= 1;
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
  print_log(f_index, index, reg_num, dependency, size, value, opcode, 0,
            " inject_after");
}

#pragma endregion

void fault_inject_finish() {
#if PRINT_MESSAGE
  fprintf(log_file, "[Fault Injector] RTL-Core Finish!\n");
#endif

  fprintf(log_file, "[Fault Injector] determine=%d\n", determine_count);

  // Save result file.
  fclose(log_file);
}
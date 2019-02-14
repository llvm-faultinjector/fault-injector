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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define MAX_INDEX 65535
#define PRINT_MESSAGE 1

typedef uint32_t major_type;

static int run_fault_injection = 1;
// 미리 테이블을 만듦
static int make_pre_table = 0;
// 대상 레지스터 수
static int count_of_index = 0;
// 제비뽑기
static int drawing_lots = 0;
// 고정된 특정 비트를 플립시킬지의 여부
static int fix_flipbitpos = 0;
static int flipbitpos = -1;
static int injection_type = 0;

static int check_index[MAX_INDEX];
static int check_count_require[MAX_INDEX];
static int check_count[MAX_INDEX];

static int profile_count = 0;
static int profile_index[MAX_INDEX * 4];
static int profile_value[MAX_INDEX * 4];

void init(uint32_t _count_of_index)
{
#if PRINT_MESSAGE
  printf("fault-injector rtl-core init!\n");
#endif

  srand(time(NULL));
  count_of_index = _count_of_index;

  //
  // TODO: Read profile.
  //

  //
  // TODO: Initialize vars.
  //

  if (run_fault_injection == 0)
    return;

  if (make_pre_table == 1) {
    if (drawing_lots == 1) {
      int chk = rand() % count_of_index;
      check_index[chk] = 1;
      drawing_lots = 0;
    }
  } else {
    drawing_lots = 1;
  }
}

static int determine(double rate)
{
  return (double)rand() / RAND_MAX >= rate;
}

void fault_inject_profile(
  major_type index, 
  major_type value) {
  if (profile_count == MAX_INDEX * 4)
    return;
  profile_index[profile_count] = index;
  profile_value[profile_count] = value;
  profile_count++;
}

major_type fault_inject_determine(
  major_type index,
  major_type dependency)
{
  if (run_fault_injection == 0)
    return 0;
  if (drawing_lots == 1)
    return determine((double)1 / count_of_index);
  if (make_pre_table == 1) {
    check_count[index]++;
    if (check_count_require[index] == check_count[index] - 1)
      return check_index[index];
  }
  return 0;
}

static int get_fault_inject_pos() {
  int pos = flipbitpos;
  if (fix_flipbitpos == 0) {
    pos = rand() % (sizeof(major_type) * 8);
  }
  return pos;
}

major_type fault_inject_flipbit(
  major_type value) {
  return value ^ (1 << get_fault_inject_pos());
}

major_type fault_inject_set0(
  major_type value) {
  return value & ~(1 << get_fault_inject_pos());
}

major_type fault_inject_set1(
  major_type value) {
  return value | (1 << get_fault_inject_pos());
}

major_type fault_inject(
  major_type value) {
  switch (injection_type) {
  case 0: return fault_inject_flipbit(value);
  case 1: return fault_inject_set0(value);
  case 2: return fault_inject_set1(value);
  }
  return value;
}

void finish()
{
#if PRINT_MESSAGE
  printf("fault-injector rtl-core finish!\n");
#endif

  //
  // TODO: Save result file.
  //
}
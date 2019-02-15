; ModuleID = 'rtl-core.c'
source_filename = "rtl-core.c"
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.14.26431"

%struct.__crt_locale_pointers = type { %struct.__crt_locale_data*, %struct.__crt_multibyte_data* }
%struct.__crt_locale_data = type opaque
%struct.__crt_multibyte_data = type opaque
%struct._iobuf = type { i8* }

$sprintf = comdat any

$vsprintf = comdat any

$_snprintf = comdat any

$_vsnprintf = comdat any

$printf = comdat any

$_vsprintf_l = comdat any

$_vsnprintf_l = comdat any

$__local_stdio_printf_options = comdat any

$_vfprintf_l = comdat any

$"??_C@_0CB@FILNMOAB@?$FLFault?5Injector?$FN?5RTL?9Core?5Init?$CB?6@" = comdat any

$"??_C@_0GG@NGLEGGBD@?$FLFault?5Injector?$FN?5f_index?$DN?$CF02d?0?5i@" = comdat any

$"??_C@_04JFFKLGJF@?$CF02X?$AA@" = comdat any

$"??_C@_01EEMJAFIK@?6?$AA@" = comdat any

$"??_C@_0CD@NKMIAIDG@?$FLFault?5Injector?$FN?5RTL?9Core?5Finish@" = comdat any

@"??_C@_0CB@FILNMOAB@?$FLFault?5Injector?$FN?5RTL?9Core?5Init?$CB?6@" = linkonce_odr dso_local unnamed_addr constant [33 x i8] c"[Fault Injector] RTL-Core Init!\0A\00", comdat, align 1
@count_of_index = internal global i32 0, align 4
@run_fault_injection = internal global i32 0, align 4
@make_pre_table = internal global i32 0, align 4
@drawing_lots = internal global i32 0, align 4
@check_index = internal global [65535 x i32] zeroinitializer, align 16
@"??_C@_0GG@NGLEGGBD@?$FLFault?5Injector?$FN?5f_index?$DN?$CF02d?0?5i@" = linkonce_odr dso_local unnamed_addr constant [102 x i8] c"[Fault Injector] f_index=%02d, index=%02d, reg_num=%02d, dependency=%d, opcode=%7s, size=%02d, value=\00", comdat, align 1
@"??_C@_04JFFKLGJF@?$CF02X?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"%02X\00", comdat, align 1
@"??_C@_01EEMJAFIK@?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [2 x i8] c"\0A\00", comdat, align 1
@check_count = internal global [65535 x i32] zeroinitializer, align 16
@check_count_require = internal global [65535 x i32] zeroinitializer, align 16
@injection_type = internal global i32 0, align 4
@"??_C@_0CD@NKMIAIDG@?$FLFault?5Injector?$FN?5RTL?9Core?5Finish@" = linkonce_odr dso_local unnamed_addr constant [35 x i8] c"[Fault Injector] RTL-Core Finish!\0A\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@flipbitpos = internal global i32 -1, align 4
@fix_flipbitpos = internal global i32 0, align 4

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @sprintf(i8*, i8*, ...) #0 comdat {
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i8*, align 8
  store i8* %1, i8** %3, align 8
  store i8* %0, i8** %4, align 8
  %7 = bitcast i8** %6 to i8*
  call void @llvm.va_start(i8* %7)
  %8 = load i8*, i8** %6, align 8
  %9 = load i8*, i8** %3, align 8
  %10 = load i8*, i8** %4, align 8
  %11 = call i32 @_vsprintf_l(i8* %10, i8* %9, %struct.__crt_locale_pointers* null, i8* %8)
  store i32 %11, i32* %5, align 4
  %12 = bitcast i8** %6 to i8*
  call void @llvm.va_end(i8* %12)
  %13 = load i32, i32* %5, align 4
  ret i32 %13
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @vsprintf(i8*, i8*, i8*) #0 comdat {
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8*, align 8
  store i8* %2, i8** %4, align 8
  store i8* %1, i8** %5, align 8
  store i8* %0, i8** %6, align 8
  %7 = load i8*, i8** %4, align 8
  %8 = load i8*, i8** %5, align 8
  %9 = load i8*, i8** %6, align 8
  %10 = call i32 @_vsnprintf_l(i8* %9, i64 -1, i8* %8, %struct.__crt_locale_pointers* null, i8* %7)
  ret i32 %10
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_snprintf(i8*, i64, i8*, ...) #0 comdat {
  %4 = alloca i8*, align 8
  %5 = alloca i64, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i32, align 4
  %8 = alloca i8*, align 8
  store i8* %2, i8** %4, align 8
  store i64 %1, i64* %5, align 8
  store i8* %0, i8** %6, align 8
  %9 = bitcast i8** %8 to i8*
  call void @llvm.va_start(i8* %9)
  %10 = load i8*, i8** %8, align 8
  %11 = load i8*, i8** %4, align 8
  %12 = load i64, i64* %5, align 8
  %13 = load i8*, i8** %6, align 8
  %14 = call i32 @_vsnprintf(i8* %13, i64 %12, i8* %11, i8* %10)
  store i32 %14, i32* %7, align 4
  %15 = bitcast i8** %8 to i8*
  call void @llvm.va_end(i8* %15)
  %16 = load i32, i32* %7, align 4
  ret i32 %16
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_vsnprintf(i8*, i64, i8*, i8*) #0 comdat {
  %5 = alloca i8*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i64, align 8
  %8 = alloca i8*, align 8
  store i8* %3, i8** %5, align 8
  store i8* %2, i8** %6, align 8
  store i64 %1, i64* %7, align 8
  store i8* %0, i8** %8, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load i8*, i8** %6, align 8
  %11 = load i64, i64* %7, align 8
  %12 = load i8*, i8** %8, align 8
  %13 = call i32 @_vsnprintf_l(i8* %12, i64 %11, i8* %10, %struct.__crt_locale_pointers* null, i8* %9)
  ret i32 %13
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fault_inject_init(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @"??_C@_0CB@FILNMOAB@?$FLFault?5Injector?$FN?5RTL?9Core?5Init?$CB?6@", i32 0, i32 0))
  %5 = call i64 @time(i64* null)
  %6 = trunc i64 %5 to i32
  call void @srand(i32 %6)
  %7 = load i32, i32* %2, align 4
  store i32 %7, i32* @count_of_index, align 4
  %8 = load i32, i32* @run_fault_injection, align 4
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %1
  br label %26

; <label>:11:                                     ; preds = %1
  %12 = load i32, i32* @make_pre_table, align 4
  %13 = icmp eq i32 %12, 1
  br i1 %13, label %14, label %25

; <label>:14:                                     ; preds = %11
  %15 = load i32, i32* @drawing_lots, align 4
  %16 = icmp eq i32 %15, 1
  br i1 %16, label %17, label %24

; <label>:17:                                     ; preds = %14
  %18 = call i32 @rand()
  %19 = load i32, i32* @count_of_index, align 4
  %20 = srem i32 %18, %19
  store i32 %20, i32* %3, align 4
  %21 = load i32, i32* %3, align 4
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds [65535 x i32], [65535 x i32]* @check_index, i64 0, i64 %22
  store i32 1, i32* %23, align 4
  store i32 0, i32* @drawing_lots, align 4
  br label %24

; <label>:24:                                     ; preds = %17, %14
  br label %26

; <label>:25:                                     ; preds = %11
  store i32 1, i32* @drawing_lots, align 4
  br label %26

; <label>:26:                                     ; preds = %10, %25, %24
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @printf(i8*, ...) #0 comdat {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  %4 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %5 = bitcast i8** %4 to i8*
  call void @llvm.va_start(i8* %5)
  %6 = load i8*, i8** %4, align 8
  %7 = load i8*, i8** %2, align 8
  %8 = call %struct._iobuf* @__acrt_iob_func(i32 1)
  %9 = call i32 @_vfprintf_l(%struct._iobuf* %8, i8* %7, %struct.__crt_locale_pointers* null, i8* %6)
  store i32 %9, i32* %3, align 4
  %10 = bitcast i8** %4 to i8*
  call void @llvm.va_end(i8* %10)
  %11 = load i32, i32* %3, align 4
  ret i32 %11
}

declare dso_local void @srand(i32) #1

; Function Attrs: noinline nounwind optnone uwtable
define internal i64 @time(i64*) #0 {
  %2 = alloca i64*, align 8
  store i64* %0, i64** %2, align 8
  %3 = load i64*, i64** %2, align 8
  %4 = call i64 @_time64(i64* %3)
  ret i64 %4
}

declare dso_local i32 @rand() #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fault_inject_trace(i32, i32, i32, i32, i32, i8*, i8*) #0 {
  %8 = alloca i8*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  store i8* %6, i8** %8, align 8
  store i8* %5, i8** %9, align 8
  store i32 %4, i32* %10, align 4
  store i32 %3, i32* %11, align 4
  store i32 %2, i32* %12, align 4
  store i32 %1, i32* %13, align 4
  store i32 %0, i32* %14, align 4
  %17 = load i32, i32* %10, align 4
  %18 = load i8*, i8** %8, align 8
  %19 = load i32, i32* %11, align 4
  %20 = load i32, i32* %12, align 4
  %21 = load i32, i32* %13, align 4
  %22 = load i32, i32* %14, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([102 x i8], [102 x i8]* @"??_C@_0GG@NGLEGGBD@?$FLFault?5Injector?$FN?5f_index?$DN?$CF02d?0?5i@", i32 0, i32 0), i32 %22, i32 %21, i32 %20, i32 %19, i8* %18, i32 %17)
  %24 = load i32, i32* %10, align 4
  %25 = udiv i32 %24, 8
  store i32 %25, i32* %15, align 4
  store i32 0, i32* %16, align 4
  br label %26

; <label>:26:                                     ; preds = %41, %7
  %27 = load i32, i32* %16, align 4
  %28 = load i32, i32* %15, align 4
  %29 = icmp slt i32 %27, %28
  br i1 %29, label %30, label %44

; <label>:30:                                     ; preds = %26
  %31 = load i8*, i8** %9, align 8
  %32 = load i32, i32* %15, align 4
  %33 = load i32, i32* %16, align 4
  %34 = sub nsw i32 %32, %33
  %35 = sub nsw i32 %34, 1
  %36 = sext i32 %35 to i64
  %37 = getelementptr inbounds i8, i8* %31, i64 %36
  %38 = load i8, i8* %37, align 1
  %39 = sext i8 %38 to i32
  %40 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @"??_C@_04JFFKLGJF@?$CF02X?$AA@", i32 0, i32 0), i32 %39)
  br label %41

; <label>:41:                                     ; preds = %30
  %42 = load i32, i32* %16, align 4
  %43 = add nsw i32 %42, 1
  store i32 %43, i32* %16, align 4
  br label %26

; <label>:44:                                     ; preds = %26
  %45 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @fault_inject_determine(i32, i32, i32, i32) #0 {
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 %3, i32* %6, align 4
  store i32 %2, i32* %7, align 4
  store i32 %1, i32* %8, align 4
  store i32 %0, i32* %9, align 4
  %10 = load i32, i32* @run_fault_injection, align 4
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %13

; <label>:12:                                     ; preds = %4
  store i32 0, i32* %5, align 4
  br label %47

; <label>:13:                                     ; preds = %4
  %14 = load i32, i32* @drawing_lots, align 4
  %15 = icmp eq i32 %14, 1
  br i1 %15, label %16, label %21

; <label>:16:                                     ; preds = %13
  %17 = load i32, i32* @count_of_index, align 4
  %18 = sitofp i32 %17 to double
  %19 = fdiv double 1.000000e+00, %18
  %20 = call i32 @determine(double %19)
  store i32 %20, i32* %5, align 4
  br label %47

; <label>:21:                                     ; preds = %13
  %22 = load i32, i32* @make_pre_table, align 4
  %23 = icmp eq i32 %22, 1
  br i1 %23, label %24, label %46

; <label>:24:                                     ; preds = %21
  %25 = load i32, i32* %8, align 4
  %26 = zext i32 %25 to i64
  %27 = getelementptr inbounds [65535 x i32], [65535 x i32]* @check_count, i64 0, i64 %26
  %28 = load i32, i32* %27, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* %27, align 4
  %30 = load i32, i32* %8, align 4
  %31 = zext i32 %30 to i64
  %32 = getelementptr inbounds [65535 x i32], [65535 x i32]* @check_count_require, i64 0, i64 %31
  %33 = load i32, i32* %32, align 4
  %34 = load i32, i32* %8, align 4
  %35 = zext i32 %34 to i64
  %36 = getelementptr inbounds [65535 x i32], [65535 x i32]* @check_count, i64 0, i64 %35
  %37 = load i32, i32* %36, align 4
  %38 = sub nsw i32 %37, 1
  %39 = icmp eq i32 %33, %38
  br i1 %39, label %40, label %45

; <label>:40:                                     ; preds = %24
  %41 = load i32, i32* %8, align 4
  %42 = zext i32 %41 to i64
  %43 = getelementptr inbounds [65535 x i32], [65535 x i32]* @check_index, i64 0, i64 %42
  %44 = load i32, i32* %43, align 4
  store i32 %44, i32* %5, align 4
  br label %47

; <label>:45:                                     ; preds = %24
  br label %46

; <label>:46:                                     ; preds = %45, %21
  store i32 0, i32* %5, align 4
  br label %47

; <label>:47:                                     ; preds = %46, %40, %16, %12
  %48 = load i32, i32* %5, align 4
  ret i32 %48
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @determine(double) #0 {
  %2 = alloca double, align 8
  store double %0, double* %2, align 8
  %3 = call i32 @rand()
  %4 = sitofp i32 %3 to double
  %5 = fdiv double %4, 3.276700e+04
  %6 = load double, double* %2, align 8
  %7 = fcmp oge double %5, %6
  %8 = zext i1 %7 to i32
  ret i32 %8
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fault_inject_flipbit(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = call i32 @get_fault_inject_pos()
  %4 = shl i32 1, %3
  %5 = load i8*, i8** %2, align 8
  %6 = load i8, i8* %5, align 1
  %7 = sext i8 %6 to i32
  %8 = xor i32 %7, %4
  %9 = trunc i32 %8 to i8
  store i8 %9, i8* %5, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fault_inject_set0(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = call i32 @get_fault_inject_pos()
  %4 = shl i32 1, %3
  %5 = xor i32 %4, -1
  %6 = load i8*, i8** %2, align 8
  %7 = load i8, i8* %6, align 1
  %8 = sext i8 %7 to i32
  %9 = and i32 %8, %5
  %10 = trunc i32 %9 to i8
  store i8 %10, i8* %6, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fault_inject_set1(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = call i32 @get_fault_inject_pos()
  %4 = shl i32 1, %3
  %5 = load i8*, i8** %2, align 8
  %6 = load i8, i8* %5, align 1
  %7 = sext i8 %6 to i32
  %8 = or i32 %7, %4
  %9 = trunc i32 %8 to i8
  store i8 %9, i8* %5, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fault_inject(i32, i32, i32, i32, i32, i8*, i8*) #0 {
  %8 = alloca i8*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  store i8* %6, i8** %8, align 8
  store i8* %5, i8** %9, align 8
  store i32 %4, i32* %10, align 4
  store i32 %3, i32* %11, align 4
  store i32 %2, i32* %12, align 4
  store i32 %1, i32* %13, align 4
  store i32 %0, i32* %14, align 4
  %15 = load i32, i32* @injection_type, align 4
  switch i32 %15, label %22 [
    i32 0, label %16
    i32 1, label %18
    i32 2, label %20
  ]

; <label>:16:                                     ; preds = %7
  %17 = load i8*, i8** %9, align 8
  call void @fault_inject_flipbit(i8* %17)
  br label %22

; <label>:18:                                     ; preds = %7
  %19 = load i8*, i8** %9, align 8
  call void @fault_inject_set0(i8* %19)
  br label %22

; <label>:20:                                     ; preds = %7
  %21 = load i8*, i8** %9, align 8
  call void @fault_inject_set1(i8* %21)
  br label %22

; <label>:22:                                     ; preds = %7, %20, %18, %16
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fault_inject_finish() #0 {
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @"??_C@_0CD@NKMIAIDG@?$FLFault?5Injector?$FN?5RTL?9Core?5Finish@", i32 0, i32 0))
  ret void
}

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #2

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_vsprintf_l(i8*, i8*, %struct.__crt_locale_pointers*, i8*) #0 comdat {
  %5 = alloca i8*, align 8
  %6 = alloca %struct.__crt_locale_pointers*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  store i8* %3, i8** %5, align 8
  store %struct.__crt_locale_pointers* %2, %struct.__crt_locale_pointers** %6, align 8
  store i8* %1, i8** %7, align 8
  store i8* %0, i8** %8, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load %struct.__crt_locale_pointers*, %struct.__crt_locale_pointers** %6, align 8
  %11 = load i8*, i8** %7, align 8
  %12 = load i8*, i8** %8, align 8
  %13 = call i32 @_vsnprintf_l(i8* %12, i64 -1, i8* %11, %struct.__crt_locale_pointers* %10, i8* %9)
  ret i32 %13
}

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #2

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_vsnprintf_l(i8*, i64, i8*, %struct.__crt_locale_pointers*, i8*) #0 comdat {
  %6 = alloca i8*, align 8
  %7 = alloca %struct.__crt_locale_pointers*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i64, align 8
  %10 = alloca i8*, align 8
  %11 = alloca i32, align 4
  store i8* %4, i8** %6, align 8
  store %struct.__crt_locale_pointers* %3, %struct.__crt_locale_pointers** %7, align 8
  store i8* %2, i8** %8, align 8
  store i64 %1, i64* %9, align 8
  store i8* %0, i8** %10, align 8
  %12 = load i8*, i8** %6, align 8
  %13 = load %struct.__crt_locale_pointers*, %struct.__crt_locale_pointers** %7, align 8
  %14 = load i8*, i8** %8, align 8
  %15 = load i64, i64* %9, align 8
  %16 = load i8*, i8** %10, align 8
  %17 = call i64* @__local_stdio_printf_options()
  %18 = load i64, i64* %17, align 8
  %19 = or i64 %18, 1
  %20 = call i32 @__stdio_common_vsprintf(i64 %19, i8* %16, i64 %15, i8* %14, %struct.__crt_locale_pointers* %13, i8* %12)
  store i32 %20, i32* %11, align 4
  %21 = load i32, i32* %11, align 4
  %22 = icmp slt i32 %21, 0
  br i1 %22, label %23, label %24

; <label>:23:                                     ; preds = %5
  br label %26

; <label>:24:                                     ; preds = %5
  %25 = load i32, i32* %11, align 4
  br label %26

; <label>:26:                                     ; preds = %24, %23
  %27 = phi i32 [ -1, %23 ], [ %25, %24 ]
  ret i32 %27
}

declare dso_local i32 @__stdio_common_vsprintf(i64, i8*, i64, i8*, %struct.__crt_locale_pointers*, i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i64* @__local_stdio_printf_options() #0 comdat {
  ret i64* @__local_stdio_printf_options._OptionsStorage
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_vfprintf_l(%struct._iobuf*, i8*, %struct.__crt_locale_pointers*, i8*) #0 comdat {
  %5 = alloca i8*, align 8
  %6 = alloca %struct.__crt_locale_pointers*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca %struct._iobuf*, align 8
  store i8* %3, i8** %5, align 8
  store %struct.__crt_locale_pointers* %2, %struct.__crt_locale_pointers** %6, align 8
  store i8* %1, i8** %7, align 8
  store %struct._iobuf* %0, %struct._iobuf** %8, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load %struct.__crt_locale_pointers*, %struct.__crt_locale_pointers** %6, align 8
  %11 = load i8*, i8** %7, align 8
  %12 = load %struct._iobuf*, %struct._iobuf** %8, align 8
  %13 = call i64* @__local_stdio_printf_options()
  %14 = load i64, i64* %13, align 8
  %15 = call i32 @__stdio_common_vfprintf(i64 %14, %struct._iobuf* %12, i8* %11, %struct.__crt_locale_pointers* %10, i8* %9)
  ret i32 %15
}

declare dso_local %struct._iobuf* @__acrt_iob_func(i32) #1

declare dso_local i32 @__stdio_common_vfprintf(i64, %struct._iobuf*, i8*, %struct.__crt_locale_pointers*, i8*) #1

declare dso_local i64 @_time64(i64*) #1

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @get_fault_inject_pos() #0 {
  %1 = alloca i32, align 4
  %2 = load i32, i32* @flipbitpos, align 4
  store i32 %2, i32* %1, align 4
  %3 = load i32, i32* @fix_flipbitpos, align 4
  %4 = icmp eq i32 %3, 0
  br i1 %4, label %5, label %10

; <label>:5:                                      ; preds = %0
  %6 = call i32 @rand()
  %7 = sext i32 %6 to i64
  %8 = urem i64 %7, 8
  %9 = trunc i64 %8 to i32
  store i32 %9, i32* %1, align 4
  br label %10

; <label>:10:                                     ; preds = %5, %0
  %11 = load i32, i32* %1, align 4
  ret i32 %11
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 7.0.1 (tags/RELEASE_701/final)"}

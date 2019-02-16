; ModuleID = 'a.ll'
source_filename = "a.c"
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

$"??_C@_02DPKJAMEF@?$CFd?$AA@" = comdat any

@"??_C@_02DPKJAMEF@?$CFd?$AA@" = linkonce_odr dso_local unnamed_addr constant [3 x i8] c"%d\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@fault_inject_load_namestr = internal constant [5 x i8] c"load\00"
@fault_inject_icmp_namestr = internal constant [5 x i8] c"icmp\00"
@fault_inject_add_namestr = internal constant [4 x i8] c"add\00"
@fault_inject_call_namestr = internal constant [5 x i8] c"call\00"

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
define dso_local i32 @main(i32, i8**) #0 {
  call void @fault_inject_init()
  %tmploc_8 = alloca i32
  %tmploc_7 = alloca i32
  %tmploc_6 = alloca i32
  %tmploc_5 = alloca i32
  %tmploc_4 = alloca i32
  %tmploc_3 = alloca i32
  %tmploc_2 = alloca i32
  %tmploc_1 = alloca i32
  %3 = alloca i32, align 4
  %4 = alloca i8**, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i8** %1, i8*** %4, align 8
  store i32 %0, i32* %5, align 4
  store i32 0, i32* %6, align 4
  store i32 0, i32* %7, align 4
  store i32 0, i32* %8, align 4
  br label %9

; <label>:9:                                      ; preds = %17, %2
  %10 = load i32, i32* %8, align 4
  %fi = call i32 @inject_fault0(i32 4, i32 0, i32 -1, i32 -1, i32 %10, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  %11 = load i32, i32* %5, align 4
  %fi1 = call i32 @inject_fault0(i32 4, i32 1, i32 -1, i32 -1, i32 %11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  %12 = icmp slt i32 %fi, %fi1
  %fi2 = call i1 @inject_fault1(i32 4, i32 2, i32 -1, i32 -1, i1 %12, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_icmp_namestr, i32 0, i32 0))
  store i32 %fi1, i32* %tmploc_2
  %tmploc_cast_2 = bitcast i32* %tmploc_2 to i8*
  call void @fault_inject_trace(i32 4, i32 1, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  store i32 %fi, i32* %tmploc_1
  %tmploc_cast_1 = bitcast i32* %tmploc_1 to i8*
  call void @fault_inject_trace(i32 4, i32 0, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_1, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  br i1 %fi2, label %13, label %20

; <label>:13:                                     ; preds = %9
  %14 = load i32, i32* %8, align 4
  %fi3 = call i32 @inject_fault0(i32 4, i32 3, i32 -1, i32 -1, i32 %14, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  %15 = load i32, i32* %7, align 4
  %fi4 = call i32 @inject_fault0(i32 4, i32 4, i32 -1, i32 -1, i32 %15, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  %16 = add nsw i32 %fi4, %fi3
  %fi5 = call i32 @inject_fault0(i32 4, i32 5, i32 -1, i32 -1, i32 %16, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fault_inject_add_namestr, i32 0, i32 0))
  store i32 %fi4, i32* %tmploc_4
  %tmploc_cast_4 = bitcast i32* %tmploc_4 to i8*
  call void @fault_inject_trace(i32 4, i32 4, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  store i32 %fi3, i32* %tmploc_3
  %tmploc_cast_3 = bitcast i32* %tmploc_3 to i8*
  call void @fault_inject_trace(i32 4, i32 3, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_3, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  store i32 %fi5, i32* %7, align 4
  store i32 %fi5, i32* %tmploc_5
  %tmploc_cast_5 = bitcast i32* %tmploc_5 to i8*
  call void @fault_inject_trace(i32 4, i32 5, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_5, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fault_inject_add_namestr, i32 0, i32 0))
  br label %17

; <label>:17:                                     ; preds = %13
  %18 = load i32, i32* %8, align 4
  %fi6 = call i32 @inject_fault0(i32 4, i32 6, i32 -1, i32 -1, i32 %18, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  %19 = add nsw i32 %fi6, 1
  %fi7 = call i32 @inject_fault0(i32 4, i32 7, i32 -1, i32 -1, i32 %19, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fault_inject_add_namestr, i32 0, i32 0))
  store i32 %fi6, i32* %tmploc_6
  %tmploc_cast_6 = bitcast i32* %tmploc_6 to i8*
  call void @fault_inject_trace(i32 4, i32 6, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  store i32 %fi7, i32* %8, align 4
  store i32 %fi7, i32* %tmploc_7
  %tmploc_cast_7 = bitcast i32* %tmploc_7 to i8*
  call void @fault_inject_trace(i32 4, i32 7, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_7, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fault_inject_add_namestr, i32 0, i32 0))
  br label %9

; <label>:20:                                     ; preds = %9
  %21 = load i32, i32* %7, align 4
  %fi8 = call i32 @inject_fault0(i32 4, i32 8, i32 -1, i32 -1, i32 %21, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32 %fi8)
  %fi9 = call i32 @inject_fault0(i32 4, i32 9, i32 -1, i32 -1, i32 %22, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_call_namestr, i32 0, i32 0))
  store i32 %fi8, i32* %tmploc_8
  %tmploc_cast_8 = bitcast i32* %tmploc_8 to i8*
  call void @fault_inject_trace(i32 4, i32 8, i32 -1, i32 -1, i32 32, i8* %tmploc_cast_8, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  %23 = load i32, i32* %3, align 4
  %fi10 = call i32 @inject_fault0(i32 4, i32 10, i32 -1, i32 -1, i32 %23, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fault_inject_load_namestr, i32 0, i32 0))
  call void @fault_inject_finish()
  ret i32 %fi10
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

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #1

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
declare void @llvm.va_end(i8*) #1

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

declare dso_local i32 @__stdio_common_vsprintf(i64, i8*, i64, i8*, %struct.__crt_locale_pointers*, i8*) #2

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

declare dso_local %struct._iobuf* @__acrt_iob_func(i32) #2

declare dso_local i32 @__stdio_common_vfprintf(i64, %struct._iobuf*, i8*, %struct.__crt_locale_pointers*, i8*) #2

declare void @fault_inject(i32, i32, i32, i32, i32, i8*, i8*)

declare void @fault_inject_trace(i32, i32, i32, i32, i32, i8*, i8*)

define i32 @inject_fault0(i32, i32, i32, i32, i32, i8*) {
entry:
  %tmploc = alloca i32
  store i32 %4, i32* %tmploc
  %pre_cond = call i1 @fault_inject_determine(i32 %0, i32 %1, i32 %2, i32 %3)
  br i1 %pre_cond, label %inject, label %exit

inject:                                           ; preds = %entry
  %tmploc_cast = bitcast i32* %tmploc to i8*
  call void @fault_inject(i32 %0, i32 %1, i32 %2, i32 %3, i32 32, i8* %tmploc_cast, i8* %5)
  br label %exit

exit:                                             ; preds = %inject, %entry
  %updateval = load i32, i32* %tmploc
  ret i32 %updateval
}

define i1 @inject_fault1(i32, i32, i32, i32, i1, i8*) {
entry:
  %tmploc = alloca i1
  store i1 %4, i1* %tmploc
  %pre_cond = call i1 @fault_inject_determine(i32 %0, i32 %1, i32 %2, i32 %3)
  br i1 %pre_cond, label %inject, label %exit

inject:                                           ; preds = %entry
  %tmploc_cast = bitcast i1* %tmploc to i8*
  call void @fault_inject(i32 %0, i32 %1, i32 %2, i32 %3, i32 1, i8* %tmploc_cast, i8* %5)
  br label %exit

exit:                                             ; preds = %inject, %entry
  %updateval = load i1, i1* %tmploc
  ret i1 %updateval
}

declare void @fault_inject_init()

declare void @fault_inject_finish()

declare i1 @fault_inject_determine(i32, i32, i32, i32)

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 7.0.1 (tags/RELEASE_701/final)"}

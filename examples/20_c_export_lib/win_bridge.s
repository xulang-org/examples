/*
 * Windows x64 MS-ABI to System V ABI Bridge for Xu Native Library (xulib.dll)
 *
 * In Microsoft x64 ABI (used by MSVC, C# .NET, standard Windows toolchains):
 *   Arguments: RCX (arg0), RDX (arg1), R8 (arg2), R9 (arg3)
 *   Registers RDI and RSI are non-volatile and MUST be preserved.
 *
 * In Xu AMD64 ABI:
 *   Arguments: RDI (arg0), RSI (arg1), RDX (arg2), RCX (arg3)
 *   SRET Buffer: R10
 *
 * This bridge adapts standard Windows MS-ABI callers to Xu ABI functions seamlessly.
 */

.text

/* --------------------------------------------------------------------------
 * 0-argument procedures
 * -------------------------------------------------------------------------- */
.globl xu_hello_from_lib
.def xu_hello_from_lib; .scl 2; .type 32; .endef
xu_hello_from_lib:
    subq $40, %rsp
    call _xu_main_xu_hello_from_lib
    addq $40, %rsp
    ret

/* --------------------------------------------------------------------------
 * 1-argument functions: RCX -> RDI
 * -------------------------------------------------------------------------- */
.macro BRIDGE_1ARG name, target
.globl \name
.def \name; .scl 2; .type 32; .endef
\name:
    pushq %rdi
    subq $32, %rsp
    movq %rcx, %rdi
    call \target
    addq $32, %rsp
    popq %rdi
    ret
.endm

BRIDGE_1ARG xu_factorial, _xu_main_xu_factorial
BRIDGE_1ARG xu_is_even, _xu_main_xu_is_even
BRIDGE_1ARG xu_string_length, _xu_main_xu_string_length
BRIDGE_1ARG xu_print_string, _xu_main_xu_print_string
BRIDGE_1ARG xu_point_distance_sq, _xu_main_xu_point_distance_sq
BRIDGE_1ARG xu_check_mixed_record, _xu_main_xu_check_mixed_record
BRIDGE_1ARG xu_sum_slice, _xu_main_xu_sum_slice
BRIDGE_1ARG xu_sum_byte_slice, _xu_main_xu_sum_byte_slice
BRIDGE_1ARG xu_sum_f64_slice, _xu_main_xu_sum_f64_slice
BRIDGE_1ARG xu_sum_fixed_array, _xu_main_xu_sum_fixed_array
BRIDGE_1ARG xu_status_code, _xu_main_xu_status_code

/* --------------------------------------------------------------------------
 * 2-argument functions: RCX -> RDI, RDX -> RSI
 * -------------------------------------------------------------------------- */
.macro BRIDGE_2ARG name, target
.globl \name
.def \name; .scl 2; .type 32; .endef
\name:
    pushq %rdi
    pushq %rsi
    subq $40, %rsp
    movq %rcx, %rdi
    movq %rdx, %rsi
    call \target
    addq $40, %rsp
    popq %rsi
    popq %rdi
    ret
.endm

BRIDGE_2ARG xu_add_int, _xu_main_xu_add_int
BRIDGE_2ARG xu_add_i32, _xu_main_xu_add_i32
BRIDGE_2ARG xu_add_u32, _xu_main_xu_add_u32
BRIDGE_2ARG xu_rotate_left_u32, _xu_main_xu_rotate_left_u32
BRIDGE_2ARG xu_mask_u8, _xu_main_xu_mask_u8
BRIDGE_2ARG xu_add_u8, _xu_main_xu_add_u8
BRIDGE_2ARG xu_logical_and, _xu_main_xu_logical_and
BRIDGE_2ARG xu_add_f64, _xu_main_xu_add_f64
BRIDGE_2ARG xu_scale_f64, _xu_main_xu_scale_f64
BRIDGE_2ARG xu_count_char, _xu_main_xu_count_char
BRIDGE_2ARG xu_string_byte_at, _xu_main_xu_string_byte_at
BRIDGE_2ARG xu_mutate_counter, _xu_main_xu_mutate_counter

/* --------------------------------------------------------------------------
 * 3-argument functions: RCX -> RDI, RDX -> RSI, R8 -> RDX
 * -------------------------------------------------------------------------- */
.macro BRIDGE_3ARG name, target
.globl \name
.def \name; .scl 2; .type 32; .endef
\name:
    pushq %rdi
    pushq %rsi
    subq $40, %rsp
    movq %rcx, %rdi
    movq %rdx, %rsi
    movq %r8, %rdx
    call \target
    addq $40, %rsp
    popq %rsi
    popq %rdi
    ret
.endm

BRIDGE_3ARG xu_clamp_i32, _xu_main_xu_clamp_i32
BRIDGE_3ARG xu_apply_callback, _xu_main_xu_apply_callback

/* --------------------------------------------------------------------------
 * SRET return functions (returns struct / string descriptor via buffer)
 * Under Windows MS-ABI, caller allocates buffer and passes sret pointer in RCX.
 * Xu ABI expects SRET pointer in R10.
 * -------------------------------------------------------------------------- */
.globl xu_greeting
.def xu_greeting; .scl 2; .type 32; .endef
xu_greeting:
    pushq %r10
    subq $40, %rsp
    movq %rcx, %r10
    call _xu_main_xu_greeting
    movq %r10, %rax
    addq $40, %rsp
    popq %r10
    ret

.globl xu_concat_hello
.def xu_concat_hello; .scl 2; .type 32; .endef
xu_concat_hello:
    pushq %rdi
    pushq %r10
    subq $40, %rsp
    movq %rcx, %r10
    movq %rdx, %rdi
    call _xu_main_xu_concat_hello
    movq %r10, %rax
    addq $40, %rsp
    popq %r10
    popq %rdi
    ret

.globl xu_make_point
.def xu_make_point; .scl 2; .type 32; .endef
xu_make_point:
    pushq %rdi
    pushq %rsi
    pushq %r10
    subq $32, %rsp
    movq %rcx, %r10
    movq %rdx, %rdi
    movq %r8, %rsi
    call _xu_main_xu_make_point
    movq %r10, %rax
    addq $32, %rsp
    popq %r10
    popq %rsi
    popq %rdi
    ret

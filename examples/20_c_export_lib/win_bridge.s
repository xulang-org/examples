/* Microsoft x64 callers -> Xu SysV ABI. Preserve Windows nonvolatile
 * integer and SIMD registers, and keep every call site 16-byte aligned. */
.text
.macro SAVE_WINDOWS
    pushq %rdi
    pushq %rsi
    subq $200, %rsp
    movdqu %xmm6, 40(%rsp)
    movdqu %xmm7, 56(%rsp)
    movdqu %xmm8, 72(%rsp)
    movdqu %xmm9, 88(%rsp)
    movdqu %xmm10, 104(%rsp)
    movdqu %xmm11, 120(%rsp)
    movdqu %xmm12, 136(%rsp)
    movdqu %xmm13, 152(%rsp)
    movdqu %xmm14, 168(%rsp)
    movdqu %xmm15, 184(%rsp)
.endm
.macro RETURN_WINDOWS
    movdqu 40(%rsp), %xmm6
    movdqu 56(%rsp), %xmm7
    movdqu 72(%rsp), %xmm8
    movdqu 88(%rsp), %xmm9
    movdqu 104(%rsp), %xmm10
    movdqu 120(%rsp), %xmm11
    movdqu 136(%rsp), %xmm12
    movdqu 152(%rsp), %xmm13
    movdqu 168(%rsp), %xmm14
    movdqu 184(%rsp), %xmm15
    addq $200, %rsp
    popq %rsi
    popq %rdi
    ret
.endm
.macro BRIDGE name, target, argc
.globl \name
.def \name; .scl 2; .type 32; .endef
\name:
    SAVE_WINDOWS
.if \argc > 0
    movq %rcx, %rdi
.endif
.if \argc > 1
    movq %rdx, %rsi
.endif
.if \argc > 2
    movq %r8, %rdx
.endif
    call \target
    RETURN_WINDOWS
.endm
BRIDGE xu_hello_from_lib, _xu_main_xu_hello_from_lib, 0
BRIDGE xu_factorial, _xu_main_xu_factorial, 1
BRIDGE xu_is_even, _xu_main_xu_is_even, 1
BRIDGE xu_string_length, _xu_main_xu_string_length, 1
BRIDGE xu_print_string, _xu_main_xu_print_string, 1
BRIDGE xu_point_distance_sq, _xu_main_xu_point_distance_sq, 1
BRIDGE xu_check_mixed_record, _xu_main_xu_check_mixed_record, 1
BRIDGE xu_sum_slice, _xu_main_xu_sum_slice, 1
BRIDGE xu_sum_byte_slice, _xu_main_xu_sum_byte_slice, 1
BRIDGE xu_sum_f64_slice, _xu_main_xu_sum_f64_slice, 1
BRIDGE xu_sum_fixed_array, _xu_main_xu_sum_fixed_array, 1
BRIDGE xu_status_code, _xu_main_xu_status_code, 1
BRIDGE xu_add_int, _xu_main_xu_add_int, 2
BRIDGE xu_add_i32, _xu_main_xu_add_i32, 2
BRIDGE xu_add_u32, _xu_main_xu_add_u32, 2
BRIDGE xu_rotate_left_u32, _xu_main_xu_rotate_left_u32, 2
BRIDGE xu_mask_u8, _xu_main_xu_mask_u8, 2
BRIDGE xu_add_u8, _xu_main_xu_add_u8, 2
BRIDGE xu_logical_and, _xu_main_xu_logical_and, 2
BRIDGE xu_add_f64, _xu_main_xu_add_f64, 2
BRIDGE xu_scale_f64, _xu_main_xu_scale_f64, 2
BRIDGE xu_count_char, _xu_main_xu_count_char, 2
BRIDGE xu_string_byte_at, _xu_main_xu_string_byte_at, 2
BRIDGE xu_mutate_counter, _xu_main_xu_mutate_counter, 2
BRIDGE xu_clamp_i32, _xu_main_xu_clamp_i32, 3
BRIDGE xu_apply_callback, _xu_main_xu_apply_callback, 3

.globl xu_greeting
.def xu_greeting; .scl 2; .type 32; .endef
xu_greeting:
    SAVE_WINDOWS
    movq %rcx, 32(%rsp)
    movq %rcx, %r10

    call _xu_main_xu_greeting
    movq 32(%rsp), %rax
    RETURN_WINDOWS

.globl xu_concat_hello
.def xu_concat_hello; .scl 2; .type 32; .endef
xu_concat_hello:
    SAVE_WINDOWS
    movq %rcx, 32(%rsp)
    movq %rcx, %r10
    movq %rdx, %rdi
    call _xu_main_xu_concat_hello
    movq 32(%rsp), %rax
    RETURN_WINDOWS

.globl xu_make_point
.def xu_make_point; .scl 2; .type 32; .endef
xu_make_point:
    SAVE_WINDOWS
    movq %rcx, 32(%rsp)
    movq %rcx, %r10
    movq %rdx, %rdi
    movq %r8, %rsi
    call _xu_main_xu_make_point
    movq 32(%rsp), %rax
    RETURN_WINDOWS

/* Xu closure entry: RDI points to {Win64 callback, user environment};
 * RSI/RDX are the two Xu arguments. The callback uses RCX/RDX/R8. */
.globl xu_ms_callback
.def xu_ms_callback; .scl 2; .type 32; .endef
xu_ms_callback:
    pushq %r10
    movq (%rdi), %r11
    movq %rdx, %r8
    movq %rsi, %rdx
    movq 8(%rdi), %rcx
    subq $32, %rsp
    call *%r11
    addq $32, %rsp
    popq %r10
    ret

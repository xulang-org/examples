#ifndef XULIB_H
#define XULIB_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 1. Symbol Export Convention
// Under Mach-O (macOS), the C compiler automatically prepends a single leading
// underscore '_' to C identifier names, so declaring 'xu_main_foo' links to
// '_xu_main_foo'. Under ELF (Linux) or COFF (Windows), symbol names match directly.
// ============================================================================
#if defined(__APPLE__)
#define XU_SYM(name) xu_main_##name
#else
#define XU_SYM(name) _xu_main_##name
#endif

// ----------------------------------------------------------------------------
// 1. int (64-bit signed integer)
// ----------------------------------------------------------------------------
extern int64_t XU_SYM(xu_add_int)(int64_t a, int64_t b);
extern int64_t XU_SYM(xu_factorial)(int64_t n);

// ----------------------------------------------------------------------------
// 2. i32 (32-bit signed integer, promoted to 64-bit register in AAPCS64 ABI)
// ----------------------------------------------------------------------------
extern int32_t XU_SYM(xu_add_i32)(int64_t a, int64_t b);
extern int32_t XU_SYM(xu_clamp_i32)(int64_t val, int64_t min_v, int64_t max_v);

// ----------------------------------------------------------------------------
// 3. u32 (32-bit unsigned integer)
// ----------------------------------------------------------------------------
extern uint32_t XU_SYM(xu_add_u32)(uint32_t a, uint32_t b);
extern uint32_t XU_SYM(xu_rotate_left_u32)(uint32_t val, uint32_t shift);

// ----------------------------------------------------------------------------
// 4. u8 (8-bit unsigned byte)
// ----------------------------------------------------------------------------
extern uint8_t XU_SYM(xu_mask_u8)(uint8_t val, uint8_t mask);
extern uint8_t XU_SYM(xu_add_u8)(uint8_t a, uint8_t b);

// ----------------------------------------------------------------------------
// 5. bool (boolean logic)
// ----------------------------------------------------------------------------
extern uint8_t XU_SYM(xu_is_even)(int64_t n);
extern uint8_t XU_SYM(xu_logical_and)(uint8_t a, uint8_t b);

// ----------------------------------------------------------------------------
// 6. f64 (64-bit IEEE-754 double precision float)
// Transferred via general-purpose registers (X0/RAX) under the Xu ABI bit-pattern.
// ----------------------------------------------------------------------------
extern uint64_t XU_SYM(xu_add_f64)(uint64_t a_bits, uint64_t b_bits);
extern uint64_t XU_SYM(xu_scale_f64)(uint64_t val_bits, uint64_t factor_bits);

// ----------------------------------------------------------------------------
// 7. string (UTF-8 string with 3-word descriptor [cap, len, ptr])
// Passed by reference to top slot (offset 0: ptr, offset -8: len, offset -16: cap).
// ----------------------------------------------------------------------------
extern int64_t XU_SYM(xu_string_length)(int64_t *str_top);
extern int64_t XU_SYM(xu_count_char)(int64_t *str_top, int64_t ch);
extern int64_t XU_SYM(xu_string_byte_at)(int64_t *str_top, int64_t idx);
extern void    XU_SYM(xu_print_string)(int64_t *str_top);
extern void    XU_SYM(xu_greeting)(void); // Returns XuString via sret pointer in x8
extern void    XU_SYM(xu_concat_hello)(int64_t *name_top); // Returns XuString via sret x8

// ----------------------------------------------------------------------------
// 8. struct (aggregate types: passing, returning, mixed scalar fields)
// ----------------------------------------------------------------------------
extern int64_t XU_SYM(xu_point_distance_sq)(int64_t *pt_top);
extern void    XU_SYM(xu_make_point)(int64_t x, int64_t y); // Returns XuPoint2D via sret x8
extern uint8_t XU_SYM(xu_check_mixed_record)(int64_t *rec_top);

// ----------------------------------------------------------------------------
// 9. slice ([]int, []u8, []f64 dynamic array views)
// ----------------------------------------------------------------------------
extern int64_t  XU_SYM(xu_sum_slice)(int64_t *slice_top);
extern int64_t  XU_SYM(xu_sum_byte_slice)(int64_t *slice_top);
extern uint64_t XU_SYM(xu_sum_f64_slice)(int64_t *slice_top);

// ----------------------------------------------------------------------------
// 10. array ([4]int: fixed-size array)
// ----------------------------------------------------------------------------
extern int64_t XU_SYM(xu_sum_fixed_array)(int64_t *arr_top);

// ----------------------------------------------------------------------------
// 11. enum (enumeration with variants)
// ----------------------------------------------------------------------------
extern int64_t XU_SYM(xu_status_code)(int64_t *status_ptr);

// ----------------------------------------------------------------------------
// 12. $& (mutable pointer / in-place struct mutation)
// ----------------------------------------------------------------------------
extern void XU_SYM(xu_mutate_counter)(int64_t *counter_ptr, int64_t delta);

// ----------------------------------------------------------------------------
// 13. callback (fn(int, int) int: 2-word closure descriptor [env, fn_ptr])
// ----------------------------------------------------------------------------
extern int64_t XU_SYM(xu_apply_callback)(int64_t x, int64_t y, int64_t *closure_top);

// ----------------------------------------------------------------------------
// 14. void (procedure with side effects / console output)
// ----------------------------------------------------------------------------
extern void XU_SYM(xu_hello_from_lib)(void);

// ============================================================================
// 2. Xu Composite Types & Helpers
// ============================================================================
typedef struct {
    int64_t cap;
    int64_t len;
    const char *ptr;
} XuString;

typedef struct {
    int64_t y; // offset -8
    int64_t x; // offset 0 (Top Slot)
} XuPoint2D;

typedef struct {
    uint64_t flag;       // offset -16 (bool)
    uint64_t score_bits; // offset -8  (f64 IEEE bits)
    int64_t  id;         // offset 0   (int, Top Slot)
} XuMixedRecord;

typedef struct {
    int64_t cap;
    int64_t len;
    const int64_t *ptr;
} XuSliceInt;

typedef struct {
    int64_t cap;
    int64_t len;
    const uint8_t *ptr;
} XuSliceByte;

typedef struct {
    int64_t cap;
    int64_t len;
    const uint64_t *ptr;
} XuSliceF64;

typedef struct {
    int64_t e3; // offset -24
    int64_t e2; // offset -16
    int64_t e1; // offset -8
    int64_t e0; // offset 0 (Top Slot)
} XuFixedArray4;

typedef enum {
    XU_STATUS_OK = 0,
    XU_STATUS_PENDING = 1,
    XU_STATUS_FAILED = 2
} XuStatus;

typedef struct {
    int64_t val;
} XuCounter;

typedef struct {
    void *env;       // offset -8
    void *fn_ptr;    // offset 0 (Top Slot)
} XuClosure;

// ============================================================================
// 3. Ergonomic Inline C Helpers
// ============================================================================
static inline uint64_t xu_f64_to_bits(double d) {
    uint64_t u;
    memcpy(&u, &d, sizeof(u));
    return u;
}

static inline double xu_bits_to_f64(uint64_t u) {
    double d;
    memcpy(&d, &u, sizeof(d));
    return d;
}

static inline double xu_add_f64(double a, double b) {
    return xu_bits_to_f64(XU_SYM(xu_add_f64)(xu_f64_to_bits(a), xu_f64_to_bits(b)));
}

static inline double xu_scale_f64(double val, double factor) {
    return xu_bits_to_f64(XU_SYM(xu_scale_f64)(xu_f64_to_bits(val), xu_f64_to_bits(factor)));
}

static inline XuString xu_make_string(const char *s) {
    XuString xs;
    xs.cap = 0; // 0 ensures Xu's drop/free check skips deallocation
    xs.len = s ? (int64_t)strlen(s) : 0;
    xs.ptr = s ? s : "";
    return xs;
}

#define XU_STR_ARG(xs) ((int64_t *)&(xs).ptr)

static inline int64_t xu_get_string_length(const char *s) {
    XuString xs = xu_make_string(s);
    return XU_SYM(xu_string_length)(XU_STR_ARG(xs));
}

static inline int64_t xu_count_char(const char *s, char c) {
    XuString xs = xu_make_string(s);
    return XU_SYM(xu_count_char)(XU_STR_ARG(xs), (int64_t)c);
}

static inline int64_t xu_string_byte_at(const char *s, int64_t idx) {
    XuString xs = xu_make_string(s);
    return XU_SYM(xu_string_byte_at)(XU_STR_ARG(xs), idx);
}

static inline void xu_print_string(const char *s) {
    XuString xs = xu_make_string(s);
    XU_SYM(xu_print_string)(XU_STR_ARG(xs));
}

static inline int64_t xu_point_distance_sq(int64_t x, int64_t y) {
    XuPoint2D pt;
    pt.x = x;
    pt.y = y;
    return XU_SYM(xu_point_distance_sq)(&pt.x);
}

static inline XuPoint2D xu_make_point(int64_t x, int64_t y) {
    XuPoint2D out;
    int64_t *sret_ptr = &out.x;
#if defined(__aarch64__)
    __asm__ volatile (
        "mov x8, %0\n"
        "mov x0, %1\n"
        "mov x1, %2\n"
        "bl _xu_main_xu_make_point\n"
        :
        : "r"(sret_ptr), "r"(x), "r"(y)
        : "x0", "x1", "x8", "lr", "x9", "x10", "x11", "x12"
    );
#elif defined(__x86_64__)
    __asm__ volatile (
        "movq %0, %%rdi\n"
        "movq %1, %%rsi\n"
        "movq %2, %%rdx\n"
        "callq _xu_main_xu_make_point\n"
        :
        : "r"(sret_ptr), "r"(x), "r"(y)
        : "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9", "r10", "r11"
    );
#endif
    return out;
}

static inline bool xu_check_mixed_record(int64_t id, double score, bool flag) {
    XuMixedRecord rec;
    rec.id = id;
    rec.score_bits = xu_f64_to_bits(score);
    rec.flag = flag ? 1 : 0;
    return XU_SYM(xu_check_mixed_record)(&rec.id) != 0;
}

static inline int64_t xu_sum_slice(const int64_t *arr, int64_t len) {
    XuSliceInt sl;
    sl.cap = 0;
    sl.len = len;
    sl.ptr = arr;
    return XU_SYM(xu_sum_slice)((int64_t *)&sl.ptr);
}

static inline int64_t xu_sum_byte_slice(const uint8_t *bytes, int64_t len) {
    XuSliceByte sl;
    sl.cap = 0;
    sl.len = len;
    sl.ptr = bytes;
    return XU_SYM(xu_sum_byte_slice)((int64_t *)&sl.ptr);
}

static inline double xu_sum_f64_slice(const double *floats, int64_t len) {
    XuSliceF64 sl;
    sl.cap = 0;
    sl.len = len;
    sl.ptr = (const uint64_t *)floats;
    return xu_bits_to_f64(XU_SYM(xu_sum_f64_slice)((int64_t *)&sl.ptr));
}

static inline int64_t xu_sum_fixed_array(int64_t a, int64_t b, int64_t c, int64_t d) {
    XuFixedArray4 arr;
    arr.e0 = a;
    arr.e1 = b;
    arr.e2 = c;
    arr.e3 = d;
    return XU_SYM(xu_sum_fixed_array)(&arr.e0);
}

static inline int64_t xu_status_code(XuStatus s) {
    int64_t val = (int64_t)s;
    return XU_SYM(xu_status_code)(&val);
}

static inline void xu_mutate_counter(XuCounter *c, int64_t delta) {
    XU_SYM(xu_mutate_counter)(&c->val, delta);
}

typedef int64_t (*XuCallbackFn)(void *env, int64_t a, int64_t b);

static inline int64_t xu_apply_callback(int64_t x, int64_t y, XuCallbackFn fn, void *env) {
    XuClosure c;
    c.env = env;
    c.fn_ptr = (void *)fn;
    return XU_SYM(xu_apply_callback)(x, y, (int64_t *)&c.fn_ptr);
}

static inline XuString xu_get_greeting(void) {
    XuString out;
    int64_t *sret_ptr = (int64_t *)&out.ptr;
#if defined(__aarch64__)
    __asm__ volatile (
        "mov x8, %0\n"
        "bl _xu_main_xu_greeting\n"
        :
        : "r"(sret_ptr)
        : "x8", "lr", "x9", "x10", "x11", "x12"
    );
#elif defined(__x86_64__)
    __asm__ volatile (
        "movq %0, %%rdi\n"
        "callq _xu_main_xu_greeting\n"
        :
        : "r"(sret_ptr)
        : "rax", "rdi", "rcx", "rdx", "rsi", "r8", "r9", "r10", "r11"
    );
#endif
    return out;
}

static inline XuString xu_concat_hello(const char *name) {
    XuString out;
    int64_t *sret_ptr = (int64_t *)&out.ptr;
    XuString in_name = xu_make_string(name);
#if defined(__aarch64__)
    __asm__ volatile (
        "mov x8, %0\n"
        "mov x0, %1\n"
        "bl _xu_main_xu_concat_hello\n"
        :
        : "r"(sret_ptr), "r"(XU_STR_ARG(in_name))
        : "x0", "x1", "x2", "x3", "x8", "lr", "x9", "x10", "x11", "x12"
    );
#elif defined(__x86_64__)
    __asm__ volatile (
        "movq %0, %%rdi\n"
        "movq %1, %%rsi\n"
        "callq _xu_main_xu_concat_hello\n"
        :
        : "r"(sret_ptr), "r"(XU_STR_ARG(in_name))
        : "rax", "rdi", "rsi", "rcx", "rdx", "r8", "r9", "r10", "r11"
    );
#endif
    return out;
}

#ifdef __cplusplus
}
#endif

#endif // XULIB_H

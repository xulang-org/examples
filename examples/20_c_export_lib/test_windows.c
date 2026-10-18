#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// Windows Native C Host: Calling Xu Library (Static xulib.lib or Dynamic xulib.dll)
//
// Uses standard Microsoft x64 Calling Convention:
//   Arguments: RCX, RDX, R8, R9
//   Return: RAX
// Seamlessly adapted to Xu ABI via xulib's built-in Windows bridge.
// ============================================================================

// 1. int
extern int64_t xu_add_int(int64_t a, int64_t b);
extern int64_t xu_factorial(int64_t n);

// 2. i32
extern int32_t xu_add_i32(int64_t a, int64_t b);
extern int32_t xu_clamp_i32(int64_t val, int64_t min_v, int64_t max_v);

// 3. u32
extern uint32_t xu_add_u32(uint32_t a, uint32_t b);
extern uint32_t xu_rotate_left_u32(uint32_t val, uint32_t shift);

// 4. u8
extern uint8_t xu_mask_u8(uint8_t val, uint8_t mask);
extern uint8_t xu_add_u8(uint8_t a, uint8_t b);

// 5. bool
extern uint8_t xu_is_even(int64_t n);
extern uint8_t xu_logical_and(uint8_t a, uint8_t b);

// 6. f64
extern uint64_t xu_add_f64(uint64_t a_bits, uint64_t b_bits);
extern uint64_t xu_scale_f64(uint64_t val_bits, uint64_t factor_bits);

// 7. string
typedef struct {
    int64_t cap;
    int64_t len;
    const char *ptr;
} XuString;

extern int64_t xu_string_length(int64_t *str_top);
extern int64_t xu_count_char(int64_t *str_top, int64_t ch);
extern int64_t xu_string_byte_at(int64_t *str_top, int64_t idx);
extern void    xu_print_string(int64_t *str_top);
extern void   *xu_greeting(void *sret_top);
extern void   *xu_concat_hello(void *sret_top, int64_t *name_top);

// 8. struct
typedef struct {
    int64_t y;
    int64_t x;
} XuPoint2D;

typedef struct {
    uint64_t flag;
    uint64_t score_bits;
    int64_t  id;
} XuMixedRecord;

extern int64_t xu_point_distance_sq(int64_t *pt_top);
extern void   *xu_make_point(void *sret_top, int64_t x, int64_t y);
extern uint8_t xu_check_mixed_record(int64_t *rec_top);

// 9. slice
typedef struct {
    int64_t cap;
    int64_t len;
    const void *ptr;
} XuSlice;

extern int64_t  xu_sum_slice(int64_t *slice_top);
extern int64_t  xu_sum_byte_slice(int64_t *slice_top);
extern uint64_t xu_sum_f64_slice(int64_t *slice_top);

// 10. array
typedef struct {
    int64_t e3;
    int64_t e2;
    int64_t e1;
    int64_t e0;
} XuFixedArray4;

extern int64_t xu_sum_fixed_array(int64_t *arr_top);

// 11. enum
typedef enum {
    XU_STATUS_OK = 0,
    XU_STATUS_PENDING = 1,
    XU_STATUS_FAILED = 2
} XuStatus;

extern int64_t xu_status_code(int64_t *status_ptr);

// 12. &mut
typedef struct {
    int64_t val;
} XuCounter;

extern void xu_mutate_counter(int64_t *counter_ptr, int64_t delta);

// 13. callback
typedef struct {
    void *env;
    void *fn_ptr;
} XuClosure;

extern int64_t xu_apply_callback(int64_t x, int64_t y, int64_t *closure_top);

// 14. void
extern void xu_hello_from_lib(void);

// Helper functions
static inline uint64_t f64_to_bits(double d) {
    uint64_t u;
    memcpy(&u, &d, sizeof(u));
    return u;
}

static inline double bits_to_f64(uint64_t u) {
    double d;
    memcpy(&d, &u, sizeof(d));
    return d;
}

static inline XuString make_string(const char *s) {
    XuString xs;
    xs.cap = 0;
    xs.len = s ? (int64_t)strlen(s) : 0;
    xs.ptr = s ? s : "";
    return xs;
}

static int64_t win_callback_mult(void *env, int64_t a, int64_t b) {
    (void)env;
    return a * b;
}

int main(void) {
    printf("====================================================================\n");
    printf("   Windows Host Calling Xu Native Library (xulib.lib / xulib.dll)\n");
    printf("====================================================================\n\n");

    // 1. int
    printf("[1. int] Testing 64-bit integer...\n");
    int64_t sum = xu_add_int(100, 200);
    printf("    xu_add_int(100, 200) = %lld (expected 300)\n", (long long)sum);
    assert(sum == 300);

    int64_t fact = xu_factorial(6);
    printf("    xu_factorial(6) = %lld (expected 720)\n", (long long)fact);
    assert(fact == 720);

    // 2. i32
    printf("\n[2. i32] Testing 32-bit integer...\n");
    int32_t s32 = xu_add_i32(-20, 35);
    printf("    xu_add_i32(-20, 35) = %d (expected 15)\n", s32);
    assert(s32 == 15);

    int32_t clamped = xu_clamp_i32(150, 0, 100);
    printf("    xu_clamp_i32(150, 0, 100) = %d (expected 100)\n", clamped);
    assert(clamped == 100);

    // 3. u32
    printf("\n[3. u32] Testing 32-bit unsigned integer...\n");
    uint32_t u32_val = xu_add_u32(0xFFFFFF00U, 0x10U);
    printf("    xu_add_u32(0xFFFFFF00, 0x10) = 0x%08X (expected 0xFFFFFF10)\n", u32_val);
    assert(u32_val == 0xFFFFFF10U);

    uint32_t rot = xu_rotate_left_u32(1U, 4U);
    printf("    xu_rotate_left_u32(1, 4) = %u (expected 16)\n", rot);
    assert(rot == 16U);

    // 4. u8
    printf("\n[4. u8] Testing 8-bit unsigned byte...\n");
    uint8_t masked = xu_mask_u8(0xAB, 0x0F);
    printf("    xu_mask_u8(0xAB, 0x0F) = 0x%02X (expected 0x0B)\n", masked);
    assert(masked == 0x0B);

    uint8_t sum_u8 = xu_add_u8(200, 50);
    printf("    xu_add_u8(200, 50) = %u (expected 250)\n", sum_u8);
    assert(sum_u8 == 250);

    // 5. bool
    printf("\n[5. bool] Testing boolean logic...\n");
    uint8_t even42 = xu_is_even(42);
    uint8_t even43 = xu_is_even(43);
    printf("    xu_is_even(42) = %s, xu_is_even(43) = %s\n",
           even42 ? "true" : "false", even43 ? "true" : "false");
    assert(even42 == 1 && even43 == 0);

    uint8_t land = xu_logical_and(1, 0);
    printf("    xu_logical_and(true, false) = %s (expected false)\n", land ? "true" : "false");
    assert(land == 0);

    // 6. f64
    printf("\n[6. f64] Testing 64-bit IEEE float...\n");
    double f_add = bits_to_f64(xu_add_f64(f64_to_bits(1.25), f64_to_bits(2.75)));
    printf("    xu_add_f64(1.25, 2.75) = %.4f (expected 4.0000)\n", f_add);
    assert(f_add == 4.0);

    double scaled = bits_to_f64(xu_scale_f64(f64_to_bits(3.5), f64_to_bits(2.0)));
    printf("    xu_scale_f64(3.5, 2.0) = %.4f (expected 7.0000)\n", scaled);
    assert(scaled == 7.0);

    // 7. string
    printf("\n[7. string] Testing UTF-8 string descriptors and passing...\n");
    XuString test_s = make_string("Hello from Windows C Host!");
    int64_t len = xu_string_length((int64_t *)&test_s.ptr);
    printf("    xu_string_length('%s') = %lld (expected 26)\n", test_s.ptr, (long long)len);
    assert(len == 26);

    int64_t count_o = xu_count_char((int64_t *)&test_s.ptr, 'o');
    printf("    xu_count_char(..., 'o') = %lld (expected 3)\n", (long long)count_o);
    assert(count_o == 3);

    int64_t byte_at = xu_string_byte_at((int64_t *)&test_s.ptr, 0);
    printf("    xu_string_byte_at(..., 0) = %lld ('%c', expected 'H')\n", (long long)byte_at, (char)byte_at);
    assert(byte_at == 'H');

    xu_print_string((int64_t *)&test_s.ptr);

    XuString greet_out;
    xu_greeting((void *)&greet_out.ptr);
    printf("    xu_greeting() = '%s' (len: %lld)\n", greet_out.ptr, (long long)greet_out.len);
    assert(greet_out.len > 0);

    // 8. struct
    printf("\n[8. struct] Testing struct passing and returning...\n");
    XuPoint2D pt_in;
    pt_in.x = 3;
    pt_in.y = 4;
    int64_t dist_sq = xu_point_distance_sq(&pt_in.x);
    printf("    xu_point_distance_sq(3, 4) = %lld (expected 25)\n", (long long)dist_sq);
    assert(dist_sq == 25);

    XuPoint2D pt_out;
    xu_make_point((void *)&pt_out.x, 7, 24);
    printf("    xu_make_point(7, 24) = (%lld, %lld)\n", (long long)pt_out.x, (long long)pt_out.y);
    assert(pt_out.x == 7 && pt_out.y == 24);

    XuMixedRecord rec;
    rec.id = 101;
    rec.score_bits = f64_to_bits(85.5);
    rec.flag = 1;
    uint8_t rec_ok = xu_check_mixed_record(&rec.id);
    printf("    xu_check_mixed_record(id=101, score=85.5, flag=true) = %s\n", rec_ok ? "true" : "false");
    assert(rec_ok == 1);

    // 9. slice
    printf("\n[9. slice] Testing slice views ([]int, []u8, []f64)...\n");
    int64_t slice_ints[] = {10, 20, 30, 40};
    XuSlice sl_int = {0, 4, slice_ints};
    int64_t sum_sl = xu_sum_slice((int64_t *)&sl_int.ptr);
    printf("    xu_sum_slice([10, 20, 30, 40]) = %lld (expected 100)\n", (long long)sum_sl);
    assert(sum_sl == 100);

    uint8_t slice_bytes[] = {1, 2, 3, 4};
    XuSlice sl_byte = {0, 4, slice_bytes};
    int64_t sum_bytes = xu_sum_byte_slice((int64_t *)&sl_byte.ptr);
    printf("    xu_sum_byte_slice([1, 2, 3, 4]) = %lld (expected 10)\n", (long long)sum_bytes);
    assert(sum_bytes == 10);

    double slice_f64s[] = {1.25, 2.75, 6.0};
    XuSlice sl_f64 = {0, 3, slice_f64s};
    double sum_f64 = bits_to_f64(xu_sum_f64_slice((int64_t *)&sl_f64.ptr));
    printf("    xu_sum_f64_slice([1.25, 2.75, 6.0]) = %.4f (expected 10.0000)\n", sum_f64);
    assert(sum_f64 == 10.0);

    // 10. array
    printf("\n[10. array] Testing fixed-size array [4]int...\n");
    XuFixedArray4 arr;
    arr.e0 = 100;
    arr.e1 = 200;
    arr.e2 = 300;
    arr.e3 = 400;
    int64_t sum_arr = xu_sum_fixed_array(&arr.e0);
    printf("    xu_sum_fixed_array([100, 200, 300, 400]) = %lld (expected 1000)\n", (long long)sum_arr);
    assert(sum_arr == 1000);

    // 11. enum
    printf("\n[11. enum] Testing enumeration variants...\n");
    int64_t st_pending = XU_STATUS_PENDING;
    int64_t code = xu_status_code(&st_pending);
    printf("    xu_status_code(Pending) = %lld (expected 200)\n", (long long)code);
    assert(code == 200);

    // 12. &mut
    printf("\n[12. &mut] Testing mutable pointer in-place mutation...\n");
    XuCounter cnt = {50};
    xu_mutate_counter(&cnt.val, 25);
    printf("    xu_mutate_counter(&cnt{50}, 25) -> cnt.val = %lld (expected 75)\n", (long long)cnt.val);
    assert(cnt.val == 75);

    // 13. callback
    printf("\n[13. callback] Testing function pointer / closure callback...\n");
    XuClosure clos;
    clos.env = NULL;
    clos.fn_ptr = (void *)win_callback_mult;
    int64_t cb_res = xu_apply_callback(7, 8, (int64_t *)&clos.fn_ptr);
    printf("    xu_apply_callback(7, 8, mult) = %lld (expected 56)\n", (long long)cb_res);
    assert(cb_res == 56);

    // 14. void
    printf("\n[14. void] Testing procedure invocation:\n");
    xu_hello_from_lib();

    printf("\n====================================================================\n");
    printf("   >>> ALL 14 WINDOWS C DATA TYPE TESTS PASSED SUCCESSFULLY! <<<\n");
    printf("====================================================================\n");

    return 0;
}

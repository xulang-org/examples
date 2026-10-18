#include "xulib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static int64_t dynamic_callback_mult(void *env, int64_t a, int64_t b) {
    (void)env;
    return a * b;
}

int main(void) {
    printf("====================================================================\n");
    printf("   C Host Calling Xu Dynamic Library (libxulib.dylib)\n");
    printf("====================================================================\n\n");

    // ------------------------------------------------------------------------
    // 1. int (64-bit signed integer)
    // ------------------------------------------------------------------------
    printf("[1. int] Testing 64-bit integer arithmetic and recursion...\n");
    int64_t sum = XU_SYM(xu_add_int)(100, 200);
    printf("    xu_add_int(100, 200) = %lld (expected 300)\n", (long long)sum);
    assert(sum == 300);

    int64_t fact = XU_SYM(xu_factorial)(6);
    printf("    xu_factorial(6) = %lld (expected 720)\n", (long long)fact);
    assert(fact == 720);

    // ------------------------------------------------------------------------
    // 2. i32 (32-bit signed integer)
    // ------------------------------------------------------------------------
    printf("\n[2. i32] Testing 32-bit signed integer...\n");
    int32_t sum_i32 = XU_SYM(xu_add_i32)(-20, 35);
    printf("    xu_add_i32(-20, 35) = %d (expected 15)\n", sum_i32);
    assert(sum_i32 == 15);

    int32_t clamped = XU_SYM(xu_clamp_i32)(150, 0, 100);
    printf("    xu_clamp_i32(150, 0, 100) = %d (expected 100)\n", clamped);
    assert(clamped == 100);

    // ------------------------------------------------------------------------
    // 3. u32 (32-bit unsigned integer with bitwise ops)
    // ------------------------------------------------------------------------
    printf("\n[3. u32] Testing 32-bit unsigned integer and bitwise shifts...\n");
    uint32_t sum_u32 = XU_SYM(xu_add_u32)(0xFFFFFF00U, 0x10U);
    printf("    xu_add_u32(0xFFFFFF00, 0x10) = 0x%08X (expected 0xFFFFFF10)\n", sum_u32);
    assert(sum_u32 == 0xFFFFFF10U);

    uint32_t rotated = XU_SYM(xu_rotate_left_u32)(1U, 4U);
    printf("    xu_rotate_left_u32(1, 4) = %u (expected 16)\n", rotated);
    assert(rotated == 16U);

    // ------------------------------------------------------------------------
    // 4. u8 (8-bit unsigned byte)
    // ------------------------------------------------------------------------
    printf("\n[4. u8] Testing 8-bit unsigned byte mask and addition...\n");
    uint8_t masked = XU_SYM(xu_mask_u8)(0xAB, 0x0F);
    printf("    xu_mask_u8(0xAB, 0x0F) = 0x%02X (expected 0x0B)\n", masked);
    assert(masked == 0x0B);

    uint8_t sum_u8 = XU_SYM(xu_add_u8)(200, 50);
    printf("    xu_add_u8(200, 50) = %u (expected 250)\n", sum_u8);
    assert(sum_u8 == 250);

    // ------------------------------------------------------------------------
    // 5. bool (boolean logic)
    // ------------------------------------------------------------------------
    printf("\n[5. bool] Testing boolean predicates and logic gates...\n");
    uint8_t even42 = XU_SYM(xu_is_even)(42);
    uint8_t even43 = XU_SYM(xu_is_even)(43);
    printf("    xu_is_even(42) = %s, xu_is_even(43) = %s\n",
           even42 ? "true" : "false", even43 ? "true" : "false");
    assert(even42 == 1 && even43 == 0);

    uint8_t land = XU_SYM(xu_logical_and)(1, 0);
    printf("    xu_logical_and(true, false) = %s (expected false)\n",
           land ? "true" : "false");
    assert(land == 0);

    // ------------------------------------------------------------------------
    // 6. f64 (64-bit IEEE-754 double precision float)
    // ------------------------------------------------------------------------
    printf("\n[6. f64] Testing 64-bit floating point arithmetic...\n");
    double f_add = xu_add_f64(1.25, 2.75);
    printf("    xu_add_f64(1.25, 2.75) = %.4f (expected 4.0000)\n", f_add);
    assert(f_add == 4.0);

    double scaled = xu_scale_f64(3.5, 2.0);
    printf("    xu_scale_f64(3.5, 2.0) = %.4f (expected 7.0000)\n", scaled);
    assert(scaled == 7.0);

    // ------------------------------------------------------------------------
    // 7. string (UTF-8 string inspection, counting, print & return)
    // ------------------------------------------------------------------------
    printf("\n[7. string] Testing string length, counting, and passing...\n");
    const char *test_msg = "Hello, Xu Language from C Dynamic Caller!";
    int64_t len = xu_get_string_length(test_msg);
    printf("    xu_get_string_length('%s') = %lld (expected %zu)\n",
           test_msg, (long long)len, strlen(test_msg));
    assert(len == (int64_t)strlen(test_msg));

    int64_t count_a = xu_count_char("banana split", 'a');
    printf("    xu_count_char('banana split', 'a') = %lld (expected 3)\n", (long long)count_a);
    assert(count_a == 3);

    int64_t byte_val = xu_string_byte_at("XuLang", 2);
    printf("    xu_string_byte_at('XuLang', 2) = %lld ('%c', expected 'L')\n",
           (long long)byte_val, (char)byte_val);
    assert(byte_val == 'L');

    printf("    Calling xu_print_string from C:\n    ");
    xu_print_string("Message successfully routed through Xu standard runtime!");

    printf("    Receiving returned string from xu_get_greeting():\n");
    XuString greeting = xu_get_greeting();
    printf("    greeting = '%.*s' (len=%lld, cap=%lld)\n",
           (int)greeting.len, greeting.ptr, (long long)greeting.len, (long long)greeting.cap);
    assert(greeting.len > 0 && strstr(greeting.ptr, "Xu") != NULL);

    printf("    Receiving concatenated string from xu_concat_hello('Clang'):\n");
    XuString concat = xu_concat_hello("Clang");
    printf("    concat = '%.*s' (len=%lld)\n", (int)concat.len, concat.ptr, (long long)concat.len);
    assert(concat.len > 0 && strstr(concat.ptr, "Clang") != NULL);

    // ------------------------------------------------------------------------
    // 8. struct (aggregate types: passing, returning, mixed scalar fields)
    // ------------------------------------------------------------------------
    printf("\n[8. struct] Testing struct parameters, return by value, and mixed fields...\n");
    int64_t dist_sq = xu_point_distance_sq(3, 4);
    printf("    xu_point_distance_sq(3, 4) = %lld (expected 25)\n", (long long)dist_sq);
    assert(dist_sq == 25);

    XuPoint2D ret_pt = xu_make_point(7, 24);
    printf("    xu_make_point(7, 24) = Point2D{x: %lld, y: %lld}\n", (long long)ret_pt.x, (long long)ret_pt.y);
    assert(ret_pt.x == 7 && ret_pt.y == 24);

    bool check_rec = xu_check_mixed_record(101, 85.5, true);
    printf("    xu_check_mixed_record(101, 85.5, true) = %s\n", check_rec ? "true" : "false");
    assert(check_rec == true);

    // ------------------------------------------------------------------------
    // 9. slice ([]int, []u8, []f64 dynamic array views)
    // ------------------------------------------------------------------------
    printf("\n[9. slice] Testing slice views across multiple element types...\n");
    int64_t int_arr[] = {10, 20, 30, 40};
    int64_t slice_sum = xu_sum_slice(int_arr, 4);
    printf("    xu_sum_slice([10, 20, 30, 40], len=4) = %lld (expected 100)\n", (long long)slice_sum);
    assert(slice_sum == 100);

    uint8_t byte_arr[] = {1, 2, 3, 4};
    int64_t byte_sum = xu_sum_byte_slice(byte_arr, 4);
    printf("    xu_sum_byte_slice([1, 2, 3, 4], len=4) = %lld (expected 10)\n", (long long)byte_sum);
    assert(byte_sum == 10);

    double float_arr[] = {1.25, 2.75, 6.0};
    double f64_sum = xu_sum_f64_slice(float_arr, 3);
    printf("    xu_sum_f64_slice([1.25, 2.75, 6.0], len=3) = %.4f (expected 10.0000)\n", f64_sum);
    assert(f64_sum == 10.0);

    // ------------------------------------------------------------------------
    // 10. array ([4]int: fixed-size array)
    // ------------------------------------------------------------------------
    printf("\n[10. array] Testing fixed-size array [4]int...\n");
    int64_t fixed_sum = xu_sum_fixed_array(100, 200, 300, 400);
    printf("    xu_sum_fixed_array([100, 200, 300, 400]) = %lld (expected 1000)\n", (long long)fixed_sum);
    assert(fixed_sum == 1000);

    // ------------------------------------------------------------------------
    // 11. enum (enumeration with variants)
    // ------------------------------------------------------------------------
    printf("\n[11. enum] Testing enumeration variants and pattern matching...\n");
    int64_t code_ok = xu_status_code(XU_STATUS_OK);
    int64_t code_pending = xu_status_code(XU_STATUS_PENDING);
    int64_t code_failed = xu_status_code(XU_STATUS_FAILED);
    printf("    xu_status_code(Ok)=%lld, Pending=%lld, Failed=%lld\n",
           (long long)code_ok, (long long)code_pending, (long long)code_failed);
    assert(code_ok == 100 && code_pending == 200 && code_failed == 300);

    // ------------------------------------------------------------------------
    // 12. &mut (mutable pointer / in-place struct mutation)
    // ------------------------------------------------------------------------
    printf("\n[12. &mut] Testing in-place mutable reference mutation...\n");
    XuCounter cnt = { .val = 50 };
    xu_mutate_counter(&cnt, 25);
    printf("    xu_mutate_counter(&cnt{50}, 25) -> cnt.val = %lld (expected 75)\n", (long long)cnt.val);
    assert(cnt.val == 75);

    // ------------------------------------------------------------------------
    // 13. callback (fn(int, int) int closure)
    // ------------------------------------------------------------------------
    printf("\n[13. callback] Testing C function pointer passed to Xu callback...\n");
    int64_t cb_result = xu_apply_callback(7, 8, dynamic_callback_mult, NULL);
    printf("    xu_apply_callback(7, 8, dynamic_callback_mult) = %lld (expected 56)\n", (long long)cb_result);
    assert(cb_result == 56);

    // ------------------------------------------------------------------------
    // 14. void (procedure with side effects)
    // ------------------------------------------------------------------------
    printf("\n[14. void] Testing procedure invocation:\n    ");
    XU_SYM(xu_hello_from_lib)();

    printf("\n====================================================================\n");
    printf("   >>> ALL DYNAMIC LIBRARY ASSERTIONS PASSED SUCCESSFULLY! <<<\n");
    printf("====================================================================\n\n");
    return 0;
}

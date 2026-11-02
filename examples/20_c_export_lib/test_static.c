#include "xulib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static int64_t static_callback_mult(void *env, int64_t a, int64_t b) {
    (void)env;
    return a * b;
}

int main(void) {
    printf("====================================================================\n");
    printf("   C Host Statically Linked with Xu Archive (libxulib.a)\n");
    printf("====================================================================\n\n");

    // 1. int (64-bit signed integer)
    printf("[1. int] Testing 64-bit integer...\n");
    int64_t sum = XU_SYM(xu_add_int)(500, 600);
    printf("    xu_add_int(500, 600) = %lld (expected 1100)\n", (long long)sum);
    assert(sum == 1100);

    int64_t fact = XU_SYM(xu_factorial)(7);
    printf("    xu_factorial(7) = %lld (expected 5040)\n", (long long)fact);
    assert(fact == 5040);

    // 2. i32 (32-bit signed integer)
    printf("\n[2. i32] Testing 32-bit signed integer...\n");
    int32_t sum_i32 = XU_SYM(xu_add_i32)(100, -40);
    printf("    xu_add_i32(100, -40) = %d (expected 60)\n", sum_i32);
    assert(sum_i32 == 60);

    int32_t clamped = XU_SYM(xu_clamp_i32)(-5, 0, 100);
    printf("    xu_clamp_i32(-5, 0, 100) = %d (expected 0)\n", clamped);
    assert(clamped == 0);

    // 3. u32 (32-bit unsigned integer)
    printf("\n[3. u32] Testing 32-bit unsigned integer...\n");
    uint32_t sum_u32 = XU_SYM(xu_add_u32)(0x12340000U, 0x00005678U);
    printf("    xu_add_u32(0x12340000, 0x00005678) = 0x%08X (expected 0x12345678)\n", sum_u32);
    assert(sum_u32 == 0x12345678U);

    uint32_t rotated = XU_SYM(xu_rotate_left_u32)(0x80000001U, 1U);
    printf("    xu_rotate_left_u32(0x80000001, 1) = 0x%08X (expected 0x00000003)\n", rotated);
    assert(rotated == 3U);

    // 4. u8 (8-bit unsigned byte)
    printf("\n[4. u8] Testing 8-bit byte operations...\n");
    uint8_t masked = XU_SYM(xu_mask_u8)(0xFF, 0x3C);
    printf("    xu_mask_u8(0xFF, 0x3C) = 0x%02X (expected 0x3C)\n", masked);
    assert(masked == 0x3C);

    uint8_t sum_u8 = XU_SYM(xu_add_u8)(128, 127);
    printf("    xu_add_u8(128, 127) = %u (expected 255)\n", sum_u8);
    assert(sum_u8 == 255);

    // 5. bool (boolean logic)
    printf("\n[5. bool] Testing boolean logic...\n");
    uint8_t even100 = XU_SYM(xu_is_even)(100);
    printf("    xu_is_even(100) = %s (expected true)\n", even100 ? "true" : "false");
    assert(even100 == 1);

    uint8_t land = XU_SYM(xu_logical_and)(1, 1);
    printf("    xu_logical_and(true, true) = %s (expected true)\n", land ? "true" : "false");
    assert(land == 1);

    // 6. f64 (64-bit IEEE-754 double precision float)
    printf("\n[6. f64] Testing 64-bit floating point arithmetic...\n");
    double f_add = xu_add_f64(10.5, 20.25);
    printf("    xu_add_f64(10.5, 20.25) = %.4f (expected 30.7500)\n", f_add);
    assert(f_add == 30.75);

    double scaled = xu_scale_f64(4.0, 2.5);
    printf("    xu_scale_f64(4.0, 2.5) = %.4f (expected 10.0000)\n", scaled);
    assert(scaled == 10.0);

    // 7. string (UTF-8 string inspection, counting, print & return)
    printf("\n[7. string] Testing string operations under static archive...\n");
    const char *test_msg = "Static linking test for Xu language library";
    int64_t len = xu_get_string_length(test_msg);
    printf("    xu_get_string_length('%s') = %lld (expected %zu)\n",
           test_msg, (long long)len, strlen(test_msg));
    assert(len == (int64_t)strlen(test_msg));

    int64_t count_i = xu_count_char(test_msg, 'i');
    printf("    xu_count_char(test_msg, 'i') = %lld\n", (long long)count_i);
    assert(count_i == 4);

    int64_t byte_val = xu_string_byte_at(test_msg, 0);
    printf("    xu_string_byte_at(test_msg, 0) = %lld ('%c', expected 'S')\n",
           (long long)byte_val, (char)byte_val);
    assert(byte_val == 'S');

    printf("    Calling xu_print_string from C static host:\n    ");
    xu_print_string("Static linking string pass-through verified!");

    printf("    Receiving greeting from statically linked library:\n");
    XuString greeting = xu_get_greeting();
    printf("    greeting = '%.*s' (len=%lld)\n", (int)greeting.len, greeting.ptr, (long long)greeting.len);
    assert(greeting.len > 0 && strstr(greeting.ptr, "Xu") != NULL);

    printf("    Receiving concatenated string from statically linked library:\n");
    XuString concat = xu_concat_hello("StaticHost");
    printf("    concat = '%.*s' (len=%lld)\n", (int)concat.len, concat.ptr, (long long)concat.len);
    assert(concat.len > 0 && strstr(concat.ptr, "StaticHost") != NULL);

    // ------------------------------------------------------------------------
    // 8. struct (aggregate types: passing, returning, mixed scalar fields)
    // ------------------------------------------------------------------------
    printf("\n[8. struct] Testing struct parameters, return by value, and mixed fields...\n");
    int64_t dist_sq = xu_point_distance_sq(6, 8);
    printf("    xu_point_distance_sq(6, 8) = %lld (expected 100)\n", (long long)dist_sq);
    assert(dist_sq == 100);

    XuPoint2D ret_pt = xu_make_point(3, 4);
    printf("    xu_make_point(3, 4) = Point2D{x: %lld, y: %lld}\n", (long long)ret_pt.x, (long long)ret_pt.y);
    assert(ret_pt.x == 3 && ret_pt.y == 4);

    bool check_rec = xu_check_mixed_record(202, 92.0, true);
    printf("    xu_check_mixed_record(202, 92.0, true) = %s\n", check_rec ? "true" : "false");
    assert(check_rec == true);

    // ------------------------------------------------------------------------
    // 9. slice ([]int, []u8, []f64 dynamic array views)
    // ------------------------------------------------------------------------
    printf("\n[9. slice] Testing slice views under static archive...\n");
    int64_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t slice_sum = xu_sum_slice(arr, 10);
    printf("    xu_sum_slice(1..10, len=10) = %lld (expected 55)\n", (long long)slice_sum);
    assert(slice_sum == 55);

    uint8_t byte_arr[] = {10, 20, 30};
    int64_t byte_sum = xu_sum_byte_slice(byte_arr, 3);
    printf("    xu_sum_byte_slice([10, 20, 30], len=3) = %lld (expected 60)\n", (long long)byte_sum);
    assert(byte_sum == 60);

    double float_arr[] = {2.5, 3.5, 4.0};
    double f64_sum = xu_sum_f64_slice(float_arr, 3);
    printf("    xu_sum_f64_slice([2.5, 3.5, 4.0], len=3) = %.4f (expected 10.0000)\n", f64_sum);
    assert(f64_sum == 10.0);

    // ------------------------------------------------------------------------
    // 10. array ([4]int: fixed-size array)
    // ------------------------------------------------------------------------
    printf("\n[10. array] Testing fixed-size array [4]int under static linking...\n");
    int64_t fixed_sum = xu_sum_fixed_array(10, 20, 30, 40);
    printf("    xu_sum_fixed_array([10, 20, 30, 40]) = %lld (expected 100)\n", (long long)fixed_sum);
    assert(fixed_sum == 100);

    // ------------------------------------------------------------------------
    // 11. enum (enumeration with variants)
    // ------------------------------------------------------------------------
    printf("\n[11. enum] Testing enumeration variants under static linking...\n");
    int64_t code_ok = xu_status_code(XU_STATUS_OK);
    int64_t code_failed = xu_status_code(XU_STATUS_FAILED);
    printf("    xu_status_code(Ok)=%lld, Failed=%lld\n", (long long)code_ok, (long long)code_failed);
    assert(code_ok == 100 && code_failed == 300);

    // ------------------------------------------------------------------------
    // 12. $& (mutable pointer / in-place struct mutation)
    // ------------------------------------------------------------------------
    printf("\n[12. $&] Testing in-place mutable reference mutation...\n");
    XuCounter cnt = { .val = 100 };
    xu_mutate_counter(&cnt, 50);
    printf("    xu_mutate_counter(&cnt{100}, 50) -> cnt.val = %lld (expected 150)\n", (long long)cnt.val);
    assert(cnt.val == 150);

    // ------------------------------------------------------------------------
    // 13. callback (fn(int, int) int closure)
    // ------------------------------------------------------------------------
    printf("\n[13. callback] Testing C function pointer passed to Xu callback...\n");
    int64_t cb_result = xu_apply_callback(9, 9, static_callback_mult, NULL);
    printf("    xu_apply_callback(9, 9, static_callback_mult) = %lld (expected 81)\n", (long long)cb_result);
    assert(cb_result == 81);

    // ------------------------------------------------------------------------
    // 14. void
    // ------------------------------------------------------------------------
    printf("\n[14. void] Testing void procedure:\n    ");
    XU_SYM(xu_hello_from_lib)();

    printf("\n====================================================================\n");
    printf("   >>> ALL STATIC LIBRARY ASSERTIONS PASSED SUCCESSFULLY! <<<\n");
    printf("====================================================================\n\n");
    return 0;
}

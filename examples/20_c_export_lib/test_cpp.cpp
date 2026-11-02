#include "xulib.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>

static int64_t cpp_mult_callback(void* env, int64_t a, int64_t b) {
    (void)env;
    return a * b;
}

int main() {
    std::cout << "====================================================================\n";
    std::cout << "   C++ Host Calling Xu Library (libxulib)\n";
    std::cout << "====================================================================\n\n";

    // 1. int (64-bit integer)
    std::cout << "[1. int] Testing 64-bit integer...\n";
    int64_t sum = XU_SYM(xu_add_int)(123, 456);
    std::cout << "    xu_add_int(123, 456) = " << sum << " (expected 579)\n";
    assert(sum == 579);

    int64_t fact = XU_SYM(xu_factorial)(5);
    std::cout << "    xu_factorial(5) = " << fact << " (expected 120)\n";
    assert(fact == 120);

    // 2. i32 (32-bit signed integer)
    std::cout << "\n[2. i32] Testing 32-bit signed integer...\n";
    int32_t sum_i32 = XU_SYM(xu_add_i32)(-50, 75);
    std::cout << "    xu_add_i32(-50, 75) = " << sum_i32 << " (expected 25)\n";
    assert(sum_i32 == 25);

    int32_t clamped = XU_SYM(xu_clamp_i32)(200, 10, 100);
    std::cout << "    xu_clamp_i32(200, 10, 100) = " << clamped << " (expected 100)\n";
    assert(clamped == 100);

    // 3. u32 (32-bit unsigned integer)
    std::cout << "\n[3. u32] Testing 32-bit unsigned integer...\n";
    uint32_t sum_u32 = XU_SYM(xu_add_u32)(0x70000000, 0x0ABC0000);
    std::cout << "    xu_add_u32 = 0x" << std::hex << sum_u32 << std::dec << "\n";
    assert(sum_u32 == 0x7ABC0000);

    uint32_t rot = XU_SYM(xu_rotate_left_u32)(1, 3);
    std::cout << "    xu_rotate_left_u32(1, 3) = " << rot << " (expected 8)\n";
    assert(rot == 8);

    // 4. u8 (8-bit byte)
    std::cout << "\n[4. u8] Testing 8-bit unsigned byte...\n";
    uint8_t masked = XU_SYM(xu_mask_u8)(0xFF, 0x55);
    std::cout << "    xu_mask_u8(0xFF, 0x55) = 0x" << std::hex << (int)masked << std::dec << "\n";
    assert(masked == 0x55);

    uint8_t sum_u8 = XU_SYM(xu_add_u8)(100, 150);
    std::cout << "    xu_add_u8(100, 150) = " << (int)sum_u8 << "\n";
    assert(sum_u8 == 250);

    // 5. bool
    std::cout << "\n[5. bool] Testing boolean logic...\n";
    bool even = XU_SYM(xu_is_even)(88);
    std::cout << "    xu_is_even(88) = " << (even ? "true" : "false") << "\n";
    assert(even == true);

    bool land = XU_SYM(xu_logical_and)(true, true);
    std::cout << "    xu_logical_and(true, true) = " << (land ? "true" : "false") << "\n";
    assert(land == true);

    // 6. f64
    std::cout << "\n[6. f64] Testing 64-bit floating point...\n";
    double f_add = xu_add_f64(3.14159, 2.71828);
    std::cout << "    xu_add_f64(3.14159, 2.71828) = " << f_add << "\n";
    assert(std::fabs(f_add - 5.85987) < 1e-4);

    double scaled = xu_scale_f64(2.5, 3.0);
    std::cout << "    xu_scale_f64(2.5, 3.0) = " << scaled << " (expected 7.5)\n";
    assert(std::fabs(scaled - 7.5) < 1e-9);

    // 7. string
    std::cout << "\n[7. string] Testing string passing and returns...\n";
    std::string cpp_str = "Greetings from Modern C++ Host!";
    int64_t len = xu_get_string_length(cpp_str.c_str());
    std::cout << "    xu_get_string_length = " << len << " (expected " << cpp_str.length() << ")\n";
    assert(len == (int64_t)cpp_str.length());

    int64_t count_e = xu_count_char(cpp_str.c_str(), 'e');
    std::cout << "    xu_count_char('" << cpp_str << "', 'e') = " << count_e << "\n";
    assert(count_e == 3);

    int64_t b_at = xu_string_byte_at(cpp_str.c_str(), 0);
    std::cout << "    xu_string_byte_at(0) = " << (char)b_at << " (expected 'G')\n";
    assert(b_at == 'G');

    std::cout << "    Calling xu_print_string from C++:\n    ";
    xu_print_string("C++ std::string bridged into Xu runtime successfully!");

    std::cout << "    Receiving returned XuString:\n";
    XuString greeting = xu_get_greeting();
    std::string greeting_cpp(greeting.ptr, greeting.len);
    std::cout << "    greeting = '" << greeting_cpp << "'\n";
    assert(greeting_cpp.find("Xu") != std::string::npos);

    XuString concat = xu_concat_hello("C++20");
    std::string concat_cpp(concat.ptr, concat.len);
    std::cout << "    concat = '" << concat_cpp << "'\n";
    assert(concat_cpp.find("C++20") != std::string::npos);

    // 8. struct
    std::cout << "\n[8. struct] Testing struct Point2D parameter, return, and mixed fields...\n";
    int64_t dist_sq = xu_point_distance_sq(5, 12);
    std::cout << "    xu_point_distance_sq(5, 12) = " << dist_sq << " (expected 169)\n";
    assert(dist_sq == 169);

    XuPoint2D ret_pt = xu_make_point(8, 15);
    std::cout << "    xu_make_point(8, 15) = Point2D{" << ret_pt.x << ", " << ret_pt.y << "}\n";
    assert(ret_pt.x == 8 && ret_pt.y == 15);

    bool check_rec = xu_check_mixed_record(303, 99.5, true);
    std::cout << "    xu_check_mixed_record(303, 99.5, true) = " << (check_rec ? "true" : "false") << "\n";
    assert(check_rec == true);

    // 9. slice
    std::cout << "\n[9. slice] Testing slice []int, []u8, []f64 from std::vector...\n";
    std::vector<int64_t> vec = {5, 10, 15, 20, 25};
    int64_t sum_vec = xu_sum_slice(vec.data(), vec.size());
    std::cout << "    xu_sum_slice([5, 10, 15, 20, 25]) = " << sum_vec << " (expected 75)\n";
    assert(sum_vec == 75);

    std::vector<uint8_t> byte_vec = {1, 2, 3, 4, 5};
    int64_t sum_bytes = xu_sum_byte_slice(byte_vec.data(), byte_vec.size());
    std::cout << "    xu_sum_byte_slice([1, 2, 3, 4, 5]) = " << sum_bytes << " (expected 15)\n";
    assert(sum_bytes == 15);

    std::vector<double> float_vec = {1.1, 2.2, 3.3};
    double sum_floats = xu_sum_f64_slice(float_vec.data(), float_vec.size());
    std::cout << "    xu_sum_f64_slice([1.1, 2.2, 3.3]) = " << sum_floats << "\n";
    assert(std::fabs(sum_floats - 6.6) < 1e-9);

    // 10. array ([4]int)
    std::cout << "\n[10. array] Testing fixed-size array [4]int...\n";
    int64_t sum_arr = xu_sum_fixed_array(1, 2, 3, 4);
    std::cout << "    xu_sum_fixed_array(1, 2, 3, 4) = " << sum_arr << " (expected 10)\n";
    assert(sum_arr == 10);

    // 11. enum
    std::cout << "\n[11. enum] Testing enumeration variants...\n";
    int64_t code_ok = xu_status_code(XU_STATUS_OK);
    int64_t code_failed = xu_status_code(XU_STATUS_FAILED);
    std::cout << "    xu_status_code(Ok)=" << code_ok << ", Failed=" << code_failed << "\n";
    assert(code_ok == 100 && code_failed == 300);

    // 12. $&
    std::cout << "\n[12. $&] Testing mutable struct reference...\n";
    XuCounter cnt = { .val = 200 };
    xu_mutate_counter(&cnt, 100);
    std::cout << "    xu_mutate_counter(&cnt{200}, 100) -> cnt.val = " << cnt.val << " (expected 300)\n";
    assert(cnt.val == 300);

    // 13. callback
    std::cout << "\n[13. callback] Testing C++ function pointer passed to Xu callback...\n";
    int64_t cb_res = xu_apply_callback(6, 7, cpp_mult_callback, nullptr);
    std::cout << "    xu_apply_callback(6, 7, cpp_mult_callback) = " << cb_res << " (expected 42)\n";
    assert(cb_res == 42);

    // 14. void
    std::cout << "\n[14. void] Testing void procedure:\n    ";
    XU_SYM(xu_hello_from_lib)();

    std::cout << "\n====================================================================\n";
    std::cout << "   >>> ALL C++ CALLER ASSERTIONS PASSED SUCCESSFULLY! <<<\n";
    std::cout << "====================================================================\n\n";
    return 0;
}

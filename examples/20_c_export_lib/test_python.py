#!/usr/bin/env python3
"""
Python Host calling Xu Dynamic Library (libxulib.dylib / libxulib.so) via ctypes.
Demonstrates cross-language interop for all core Xu data types:
  - int, i32, u32, u8, bool, f64, string, void
"""

import ctypes
import struct
import os
import sys

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Locate shared library (.dll on Windows, .dylib on macOS, .so on Linux)
    if os.name == 'nt' or sys.platform == 'win32':
        candidates = ["xulib.dll", "libxulib.dll"]
    else:
        candidates = ["libxulib.dylib", "libxulib.so", "xulib.dll"]

    lib_path = None
    for cand in candidates:
        full_p = os.path.join(script_dir, cand)
        if os.path.exists(full_p):
            lib_path = full_p
            break

    if not lib_path:
        print(f"Error: Shared library not found in {script_dir}. Run 'make lib' or './build_and_test.sh' first.")
        sys.exit(1)

    lib = ctypes.CDLL(lib_path)

    print("====================================================================")
    print(f"   Python Host Calling Xu Dynamic Library via ctypes")
    print(f"   Library: {os.path.basename(lib_path)}")
    print("====================================================================\n")

    # Helper to resolve symbol across platforms (direct name, xu_main_ prefix, or _xu_main_ prefix)
    def get_fn(name):
        for candidate_name in [name, "xu_main_" + name, "_xu_main_" + name]:
            if hasattr(lib, candidate_name):
                return getattr(lib, candidate_name)
        raise AttributeError(f"Function {name} not found in {os.path.basename(lib_path)}")

    # -------------------------------------------------------------------------
    # 1. int (64-bit signed integer)
    # -------------------------------------------------------------------------
    print("[1. int] Testing 64-bit signed integer...")
    fn_add_int = get_fn("xu_add_int")
    fn_add_int.argtypes = [ctypes.c_int64, ctypes.c_int64]
    fn_add_int.restype = ctypes.c_int64
    sum_val = fn_add_int(100, 200)
    print(f"    xu_add_int(100, 200) = {sum_val} (expected 300)")
    assert sum_val == 300

    fn_fact = get_fn("xu_factorial")
    fn_fact.argtypes = [ctypes.c_int64]
    fn_fact.restype = ctypes.c_int64
    fact_val = fn_fact(6)
    print(f"    xu_factorial(6) = {fact_val} (expected 720)")
    assert fact_val == 720

    # -------------------------------------------------------------------------
    # 2. i32 (32-bit signed integer)
    # -------------------------------------------------------------------------
    print("\n[2. i32] Testing 32-bit signed integer...")
    fn_add_i32 = get_fn("xu_add_i32")
    fn_add_i32.argtypes = [ctypes.c_int32, ctypes.c_int32]
    fn_add_i32.restype = ctypes.c_int32
    sum_i32 = fn_add_i32(-20, 35)
    print(f"    xu_add_i32(-20, 35) = {sum_i32} (expected 15)")
    assert sum_i32 == 15

    fn_clamp_i32 = get_fn("xu_clamp_i32")
    fn_clamp_i32.argtypes = [ctypes.c_int32, ctypes.c_int32, ctypes.c_int32]
    fn_clamp_i32.restype = ctypes.c_int32
    clamped = fn_clamp_i32(150, 0, 100)
    print(f"    xu_clamp_i32(150, 0, 100) = {clamped} (expected 100)")
    assert clamped == 100

    # -------------------------------------------------------------------------
    # 3. u32 (32-bit unsigned integer with bitwise ops)
    # -------------------------------------------------------------------------
    print("\n[3. u32] Testing 32-bit unsigned integer and bitwise rotate...")
    fn_add_u32 = get_fn("xu_add_u32")
    fn_add_u32.argtypes = [ctypes.c_uint32, ctypes.c_uint32]
    fn_add_u32.restype = ctypes.c_uint32
    sum_u32 = fn_add_u32(0xFFFFFF00, 0x10)
    print(f"    xu_add_u32(0xFFFFFF00, 0x10) = 0x{sum_u32:08X} (expected 0xFFFFFF10)")
    assert sum_u32 == 0xFFFFFF10

    fn_rot_u32 = get_fn("xu_rotate_left_u32")
    fn_rot_u32.argtypes = [ctypes.c_uint32, ctypes.c_uint32]
    fn_rot_u32.restype = ctypes.c_uint32
    rotated = fn_rot_u32(1, 4)
    print(f"    xu_rotate_left_u32(1, 4) = {rotated} (expected 16)")
    assert rotated == 16

    # -------------------------------------------------------------------------
    # 4. u8 (8-bit unsigned byte)
    # -------------------------------------------------------------------------
    print("\n[4. u8] Testing 8-bit unsigned byte operations...")
    fn_mask_u8 = get_fn("xu_mask_u8")
    fn_mask_u8.argtypes = [ctypes.c_uint8, ctypes.c_uint8]
    fn_mask_u8.restype = ctypes.c_uint8
    masked = fn_mask_u8(0xAB, 0x0F)
    print(f"    xu_mask_u8(0xAB, 0x0F) = 0x{masked:02X} (expected 0x0B)")
    assert masked == 0x0B

    fn_add_u8 = get_fn("xu_add_u8")
    fn_add_u8.argtypes = [ctypes.c_uint8, ctypes.c_uint8]
    fn_add_u8.restype = ctypes.c_uint8
    sum_u8 = fn_add_u8(200, 50)
    print(f"    xu_add_u8(200, 50) = {sum_u8} (expected 250)")
    assert sum_u8 == 250

    # -------------------------------------------------------------------------
    # 5. bool (boolean logic)
    # -------------------------------------------------------------------------
    print("\n[5. bool] Testing boolean logic...")
    fn_is_even = get_fn("xu_is_even")
    fn_is_even.argtypes = [ctypes.c_int64]
    fn_is_even.restype = ctypes.c_bool
    even42 = fn_is_even(42)
    even43 = fn_is_even(43)
    print(f"    xu_is_even(42) = {even42}, xu_is_even(43) = {even43}")
    assert even42 is True and even43 is False

    fn_land = get_fn("xu_logical_and")
    fn_land.argtypes = [ctypes.c_bool, ctypes.c_bool]
    fn_land.restype = ctypes.c_bool
    land = fn_land(True, False)
    print(f"    xu_logical_and(True, False) = {land} (expected False)")
    assert land is False

    # -------------------------------------------------------------------------
    # 6. f64 (64-bit IEEE-754 double precision float)
    # Under Xu ABI, f64 bit patterns are transferred in general registers (X0/RAX).
    # -------------------------------------------------------------------------
    print("\n[6. f64] Testing 64-bit IEEE-754 double precision float...")
    fn_add_f64 = get_fn("xu_add_f64")
    fn_add_f64.argtypes = [ctypes.c_uint64, ctypes.c_uint64]
    fn_add_f64.restype = ctypes.c_uint64

    fn_scale_f64 = get_fn("xu_scale_f64")
    fn_scale_f64.argtypes = [ctypes.c_uint64, ctypes.c_uint64]
    fn_scale_f64.restype = ctypes.c_uint64

    def double_to_u64(d):
        return struct.unpack('<Q', struct.pack('<d', d))[0]

    def u64_to_double(u):
        return struct.unpack('<d', struct.pack('<Q', u))[0]

    f_add = u64_to_double(fn_add_f64(double_to_u64(1.25), double_to_u64(2.75)))
    print(f"    xu_add_f64(1.25, 2.75) = {f_add:.4f} (expected 4.0000)")
    assert abs(f_add - 4.0) < 1e-9

    scaled = u64_to_double(fn_scale_f64(double_to_u64(3.5), double_to_u64(2.0)))
    print(f"    xu_scale_f64(3.5, 2.0) = {scaled:.4f} (expected 7.0000)")
    assert abs(scaled - 7.0) < 1e-9

    # -------------------------------------------------------------------------
    # 7. string (3-word descriptor [cap, len, ptr])
    # -------------------------------------------------------------------------
    print("\n[7. string] Testing string length, counting, and passing...")
    class XuStringDesc(ctypes.Structure):
        _fields_ = [
            ("cap", ctypes.c_int64),  # offset -16: 0 = unowned
            ("len", ctypes.c_int64),  # offset -8: byte length
            ("ptr", ctypes.c_char_p), # offset 0: pointer to bytes
        ]

    fn_strlen = get_fn("xu_string_length")
    fn_strlen.argtypes = [ctypes.c_void_p]
    fn_strlen.restype = ctypes.c_int64

    py_msg = b"Hello from Python ctypes caller!"
    desc = XuStringDesc(0, len(py_msg), py_msg)
    top_slot = ctypes.addressof(desc) + 16  # address of .ptr slot
    s_len = fn_strlen(top_slot)
    print(f"    xu_string_length('{py_msg.decode()}') = {s_len} (expected {len(py_msg)})")
    assert s_len == len(py_msg)

    fn_count = get_fn("xu_count_char")
    fn_count.argtypes = [ctypes.c_void_p, ctypes.c_int64]
    fn_count.restype = ctypes.c_int64
    banana_msg = b"banana split"
    b_desc = XuStringDesc(0, len(banana_msg), banana_msg)
    count_a = fn_count(ctypes.addressof(b_desc) + 16, ord('a'))
    print(f"    xu_count_char('banana split', 'a') = {count_a} (expected 3)")
    assert count_a == 3

    fn_byte_at = get_fn("xu_string_byte_at")
    fn_byte_at.argtypes = [ctypes.c_void_p, ctypes.c_int64]
    fn_byte_at.restype = ctypes.c_int64
    xulang_msg = b"XuLang"
    x_desc = XuStringDesc(0, len(xulang_msg), xulang_msg)
    b_val = fn_byte_at(ctypes.addressof(x_desc) + 16, 2)
    print(f"    xu_string_byte_at('XuLang', 2) = {b_val} ('{chr(b_val)}', expected 'L')")
    assert b_val == ord('L')

    fn_print_str = get_fn("xu_print_string")
    fn_print_str.argtypes = [ctypes.c_void_p]
    fn_print_str.restype = None
    print("    Calling xu_print_string from Python ctypes:")
    print("    ", end="")
    fn_print_str(top_slot)

    # -------------------------------------------------------------------------
    # 8. struct (aggregate types: passing, returning, mixed scalar fields)
    # -------------------------------------------------------------------------
    print("\n[8. struct] Testing struct Point2D and MixedRecord passing from Python...")
    class XuPoint2D(ctypes.Structure):
        _fields_ = [
            ("y", ctypes.c_int64), # offset -8
            ("x", ctypes.c_int64), # offset 0 (Top Slot)
        ]

    fn_pt = get_fn("xu_point_distance_sq")
    fn_pt.argtypes = [ctypes.c_void_p]
    fn_pt.restype = ctypes.c_int64
    pt = XuPoint2D(y=4, x=3)
    dist_sq = fn_pt(ctypes.addressof(pt) + 8)
    print(f"    xu_point_distance_sq(Point2D(3, 4)) = {dist_sq} (expected 25)")
    assert dist_sq == 25

    class XuMixedRecord(ctypes.Structure):
        _fields_ = [
            ("flag", ctypes.c_uint64),       # offset -16
            ("score_bits", ctypes.c_uint64), # offset -8
            ("id", ctypes.c_int64),          # offset 0 (Top Slot)
        ]

    rec = XuMixedRecord(flag=1, score_bits=double_to_u64(80.0), id=505)
    fn_check_rec = get_fn("xu_check_mixed_record")
    fn_check_rec.argtypes = [ctypes.c_void_p]
    fn_check_rec.restype = ctypes.c_bool
    rec_valid = fn_check_rec(ctypes.addressof(rec) + 16)
    print(f"    xu_check_mixed_record(505, 80.0, True) = {rec_valid}")
    assert rec_valid is True

    # -------------------------------------------------------------------------
    # 9. slice ([]int, []u8, []f64 dynamic array views)
    # -------------------------------------------------------------------------
    print("\n[9. slice] Testing slice views ([]int, []u8, []f64) from Python...")
    class XuSliceDesc(ctypes.Structure):
        _fields_ = [
            ("cap", ctypes.c_int64), # offset -16: 0 = unowned
            ("len", ctypes.c_int64), # offset -8
            ("ptr", ctypes.c_void_p), # offset 0 (Top Slot)
        ]

    c_arr = (ctypes.c_int64 * 4)(10, 20, 30, 40)
    slice_desc = XuSliceDesc(cap=0, len=4, ptr=ctypes.cast(c_arr, ctypes.c_void_p))
    fn_sum_slice = get_fn("xu_sum_slice")
    fn_sum_slice.argtypes = [ctypes.c_void_p]
    fn_sum_slice.restype = ctypes.c_int64
    sum_slice_val = fn_sum_slice(ctypes.addressof(slice_desc) + 16)
    print(f"    xu_sum_slice([10, 20, 30, 40], len=4) = {sum_slice_val} (expected 100)")
    assert sum_slice_val == 100

    byte_arr = (ctypes.c_uint8 * 4)(5, 10, 15, 20)
    byte_desc = XuSliceDesc(cap=0, len=4, ptr=ctypes.cast(byte_arr, ctypes.c_void_p))
    fn_sum_bytes = get_fn("xu_sum_byte_slice")
    fn_sum_bytes.argtypes = [ctypes.c_void_p]
    fn_sum_bytes.restype = ctypes.c_int64
    sum_bytes_val = fn_sum_bytes(ctypes.addressof(byte_desc) + 16)
    print(f"    xu_sum_byte_slice([5, 10, 15, 20], len=4) = {sum_bytes_val} (expected 50)")
    assert sum_bytes_val == 50

    f64_arr = (ctypes.c_uint64 * 3)(double_to_u64(1.5), double_to_u64(2.5), double_to_u64(6.0))
    f64_desc = XuSliceDesc(cap=0, len=3, ptr=ctypes.cast(f64_arr, ctypes.c_void_p))
    fn_sum_f64 = get_fn("xu_sum_f64_slice")
    fn_sum_f64.argtypes = [ctypes.c_void_p]
    fn_sum_f64.restype = ctypes.c_uint64
    sum_f64_val = u64_to_double(fn_sum_f64(ctypes.addressof(f64_desc) + 16))
    print(f"    xu_sum_f64_slice([1.5, 2.5, 6.0], len=3) = {sum_f64_val:.4f} (expected 10.0000)")
    assert abs(sum_f64_val - 10.0) < 1e-9

    # -------------------------------------------------------------------------
    # 10. array ([4]int: fixed-size array)
    # -------------------------------------------------------------------------
    print("\n[10. array] Testing fixed-size array [4]int from Python...")
    class XuFixedArray4(ctypes.Structure):
        _fields_ = [
            ("e3", ctypes.c_int64), # offset -24
            ("e2", ctypes.c_int64), # offset -16
            ("e1", ctypes.c_int64), # offset -8
            ("e0", ctypes.c_int64), # offset 0 (Top Slot)
        ]

    arr4 = XuFixedArray4(e3=40, e2=30, e1=20, e0=10)
    fn_sum_fixed = get_fn("xu_sum_fixed_array")
    fn_sum_fixed.argtypes = [ctypes.c_void_p]
    fn_sum_fixed.restype = ctypes.c_int64
    fixed_sum_val = fn_sum_fixed(ctypes.addressof(arr4) + 24)
    print(f"    xu_sum_fixed_array([10, 20, 30, 40]) = {fixed_sum_val} (expected 100)")
    assert fixed_sum_val == 100

    # -------------------------------------------------------------------------
    # 11. enum (enumeration with variants)
    # -------------------------------------------------------------------------
    print("\n[11. enum] Testing enumeration variants from Python...")
    fn_status = get_fn("xu_status_code")
    fn_status.argtypes = [ctypes.POINTER(ctypes.c_int64)]
    fn_status.restype = ctypes.c_int64
    tag_pending = ctypes.c_int64(1) # Pending
    code_val = fn_status(ctypes.byref(tag_pending))
    print(f"    xu_status_code(Pending) = {code_val} (expected 200)")
    assert code_val == 200

    # -------------------------------------------------------------------------
    # 12. $& (mutable pointer / in-place struct mutation)
    # -------------------------------------------------------------------------
    print("\n[12. $&] Testing mutable struct mutation from Python...")
    class XuCounter(ctypes.Structure):
        _fields_ = [("val", ctypes.c_int64)]

    counter = XuCounter(val=120)
    fn_mutate = get_fn("xu_mutate_counter")
    fn_mutate.argtypes = [ctypes.c_void_p, ctypes.c_int64]
    fn_mutate.restype = None
    fn_mutate(ctypes.byref(counter), 30)
    print(f"    xu_mutate_counter(&cnt{{120}}, 30) -> cnt.val = {counter.val} (expected 150)")
    assert counter.val == 150

    # -------------------------------------------------------------------------
    # 13. callback (fn(int, int) int closure)
    # -------------------------------------------------------------------------
    print("\n[13. callback] Testing Python function passed as Xu callback closure...")
    CALLBACK_TYPE = ctypes.CFUNCTYPE(ctypes.c_int64, ctypes.c_void_p, ctypes.c_int64, ctypes.c_int64)

    @CALLBACK_TYPE
    def py_callback_mult(env, a, b):
        return a * b

    class XuClosureDesc(ctypes.Structure):
        _fields_ = [
            ("env", ctypes.c_void_p),    # offset -8
            ("fn_ptr", ctypes.c_void_p), # offset 0 (Top Slot)
        ]

    closure_desc = XuClosureDesc(env=None, fn_ptr=ctypes.cast(py_callback_mult, ctypes.c_void_p))
    fn_apply_cb = get_fn("xu_apply_callback")
    fn_apply_cb.argtypes = [ctypes.c_int64, ctypes.c_int64, ctypes.c_void_p]
    fn_apply_cb.restype = ctypes.c_int64
    cb_val = fn_apply_cb(7, 8, ctypes.addressof(closure_desc) + 8)
    print(f"    xu_apply_callback(7, 8, py_callback_mult) = {cb_val} (expected 56)")
    assert cb_val == 56

    # -------------------------------------------------------------------------
    # 14. void (procedure with side effects)
    # -------------------------------------------------------------------------
    print("\n[14. void] Testing procedure invocation:")
    print("    ", end="")
    fn_hello = get_fn("xu_hello_from_lib")
    fn_hello.argtypes = []
    fn_hello.restype = None
    fn_hello()

    print("\n====================================================================")
    print("   >>> ALL PYTHON CTYPES ASSERTIONS PASSED SUCCESSFULLY! <<<")
    print("====================================================================\n")

if __name__ == "__main__":
    main()

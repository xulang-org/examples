// Rust FFI host calling Xu compiled library (libxulib.dylib)
use std::ffi::c_void;

#[repr(C)]
struct XuString {
    cap: i64,
    len: i64,
    ptr: *const u8,
}

impl XuString {
    fn new(s: &str) -> Self {
        XuString {
            cap: 0,
            len: s.len() as i64,
            ptr: s.as_ptr(),
        }
    }
    fn top_slot(&self) -> *const *const u8 {
        &self.ptr as *const *const u8
    }
}

#[repr(C)]
struct XuPoint2D {
    y: i64, // offset -8
    x: i64, // offset 0 (Top Slot)
}

#[repr(C)]
struct XuMixedRecord {
    flag: u64,       // offset -16
    score_bits: u64, // offset -8
    id: i64,         // offset 0 (Top Slot)
}

#[repr(C)]
struct XuSliceInt {
    cap: i64,
    len: i64,
    ptr: *const i64,
}

#[repr(C)]
struct XuSliceByte {
    cap: i64,
    len: i64,
    ptr: *const u8,
}

#[repr(C)]
struct XuSliceF64 {
    cap: i64,
    len: i64,
    ptr: *const u64,
}

#[repr(C)]
struct XuFixedArray4 {
    e3: i64,
    e2: i64,
    e1: i64,
    e0: i64, // Top Slot
}

#[repr(C)]
struct XuCounter {
    val: i64,
}

#[repr(C)]
struct XuClosure {
    env: *mut c_void,
    fn_ptr: *const c_void,
}

extern "C" {
    fn xu_main_xu_add_int(a: i64, b: i64) -> i64;
    fn xu_main_xu_factorial(n: i64) -> i64;
    fn xu_main_xu_add_i32(a: i64, b: i64) -> i32;
    fn xu_main_xu_clamp_i32(val: i64, min_v: i64, max_v: i64) -> i32;
    fn xu_main_xu_add_u32(a: u32, b: u32) -> u32;
    fn xu_main_xu_rotate_left_u32(val: u32, shift: u32) -> u32;
    fn xu_main_xu_mask_u8(val: u8, mask: u8) -> u8;
    fn xu_main_xu_add_u8(a: u8, b: u8) -> u8;
    fn xu_main_xu_is_even(n: i64) -> u8;
    fn xu_main_xu_logical_and(a: u8, b: u8) -> u8;
    fn xu_main_xu_add_f64(a_bits: u64, b_bits: u64) -> u64;
    fn xu_main_xu_scale_f64(val_bits: u64, factor_bits: u64) -> u64;
    fn xu_main_xu_string_length(str_top: *const *const u8) -> i64;
    fn xu_main_xu_count_char(str_top: *const *const u8, ch: i64) -> i64;
    fn xu_main_xu_string_byte_at(str_top: *const *const u8, idx: i64) -> i64;
    fn xu_main_xu_print_string(str_top: *const *const u8);
    fn xu_main_xu_point_distance_sq(pt_top: *const i64) -> i64;
    fn xu_main_xu_check_mixed_record(rec_top: *const i64) -> u8;
    fn xu_main_xu_sum_slice(slice_top: *const *const i64) -> i64;
    fn xu_main_xu_sum_byte_slice(slice_top: *const *const u8) -> i64;
    fn xu_main_xu_sum_f64_slice(slice_top: *const *const u64) -> u64;
    fn xu_main_xu_sum_fixed_array(arr_top: *const i64) -> i64;
    fn xu_main_xu_status_code(status_ptr: *const i64) -> i64;
    fn xu_main_xu_mutate_counter(counter_ptr: *mut i64, delta: i64);
    fn xu_main_xu_apply_callback(x: i64, y: i64, closure_top: *const *const c_void) -> i64;
    fn xu_main_xu_hello_from_lib();
}

extern "C" fn rust_mult_callback(_env: *mut c_void, a: i64, b: i64) -> i64 {
    a * b
}

fn f64_to_u64(d: f64) -> u64 {
    d.to_bits()
}

fn u64_to_f64(u: u64) -> f64 {
    f64::from_bits(u)
}

fn main() {
    println!("====================================================================");
    println!("   Rust FFI Host Calling Xu Dynamic Library (libxulib)");
    println!("====================================================================\n");

    unsafe {
        // 1. int
        println!("[1. int] Testing 64-bit integer arithmetic...");
        let sum = xu_main_xu_add_int(1000, 2345);
        println!("    xu_add_int(1000, 2345) = {} (expected 3345)", sum);
        assert_eq!(sum, 3345);

        let fact = xu_main_xu_factorial(6);
        println!("    xu_factorial(6) = {} (expected 720)", fact);
        assert_eq!(fact, 720);

        // 2. i32
        println!("\n[2. i32] Testing 32-bit signed integer...");
        let sum_i32 = xu_main_xu_add_i32(-30, 80);
        println!("    xu_add_i32(-30, 80) = {} (expected 50)", sum_i32);
        assert_eq!(sum_i32, 50);

        let clamped = xu_main_xu_clamp_i32(300, 0, 100);
        println!("    xu_clamp_i32(300, 0, 100) = {} (expected 100)", clamped);
        assert_eq!(clamped, 100);

        // 3. u32
        println!("\n[3. u32] Testing 32-bit unsigned integer...");
        let sum_u32 = xu_main_xu_add_u32(0xFF000000, 0x00FF0000);
        println!("    xu_add_u32 = 0x{:08X} (expected 0xFFFF0000)", sum_u32);
        assert_eq!(sum_u32, 0xFFFF0000);

        let rot = xu_main_xu_rotate_left_u32(1, 5);
        println!("    xu_rotate_left_u32(1, 5) = {} (expected 32)", rot);
        assert_eq!(rot, 32);

        // 4. u8
        println!("\n[4. u8] Testing 8-bit unsigned byte...");
        let masked = xu_main_xu_mask_u8(0xFE, 0x0F);
        println!("    xu_mask_u8(0xFE, 0x0F) = 0x{:02X} (expected 0x0E)", masked);
        assert_eq!(masked, 0x0E);

        let sum_u8 = xu_main_xu_add_u8(200, 55);
        println!("    xu_add_u8(200, 55) = {} (expected 255)", sum_u8);
        assert_eq!(sum_u8, 255);

        // 5. bool
        println!("\n[5. bool] Testing boolean logic...");
        let even = xu_main_xu_is_even(1024);
        println!("    xu_is_even(1024) = {}", even != 0);
        assert_eq!(even, 1);

        let land = xu_main_xu_logical_and(1, 0);
        println!("    xu_logical_and(true, false) = {}", land != 0);
        assert_eq!(land, 0);

        // 6. f64
        println!("\n[6. f64] Testing 64-bit IEEE-754 float...");
        let f_add = u64_to_f64(xu_main_xu_add_f64(f64_to_u64(10.5), f64_to_u64(20.5)));
        println!("    xu_add_f64(10.5, 20.5) = {:.4} (expected 31.0000)", f_add);
        assert!((f_add - 31.0).abs() < 1e-9);

        let scaled = u64_to_f64(xu_main_xu_scale_f64(f64_to_u64(4.5), f64_to_u64(2.0)));
        println!("    xu_scale_f64(4.5, 2.0) = {:.4} (expected 9.0000)", scaled);
        assert!((scaled - 9.0).abs() < 1e-9);

        // 7. string
        println!("\n[7. string] Testing UTF-8 string passing from Rust...");
        let msg = "Hello from Rust FFI caller!";
        let xstr = XuString::new(msg);
        let len = xu_main_xu_string_length(xstr.top_slot());
        println!("    xu_string_length('{}') = {} (expected {})", msg, len, msg.len());
        assert_eq!(len, msg.len() as i64);

        let count_o = xu_main_xu_count_char(xstr.top_slot(), b'o' as i64);
        println!("    xu_count_char('{}', 'o') = {} (expected 2)", msg, count_o);
        assert_eq!(count_o, 2);

        let byte_at = xu_main_xu_string_byte_at(xstr.top_slot(), 0);
        println!("    xu_string_byte_at(0) = {} ('{}')", byte_at, byte_at as u8 as char);
        assert_eq!(byte_at, b'H' as i64);

        println!("    Calling xu_print_string from Rust:");
        print!("    ");
        xu_main_xu_print_string(xstr.top_slot());

        // 8. struct
        println!("\n[8. struct] Testing struct Point2D and MixedRecord from Rust...");
        let pt = XuPoint2D { y: 12, x: 9 };
        let dist_sq = xu_main_xu_point_distance_sq(&pt.x);
        println!("    xu_point_distance_sq(Point2D(9, 12)) = {} (expected 225)", dist_sq);
        assert_eq!(dist_sq, 225);

        let rec = XuMixedRecord {
            flag: 1,
            score_bits: f64_to_u64(88.5),
            id: 404,
        };
        let check_rec = xu_main_xu_check_mixed_record(&rec.id);
        println!("    xu_check_mixed_record(404, 88.5, true) = {}", check_rec != 0);
        assert_eq!(check_rec, 1);

        // 9. slice
        println!("\n[9. slice] Testing slice views ([]int, []u8, []f64) from Rust...");
        let slice_data: [i64; 5] = [2, 4, 6, 8, 10];
        let slice = XuSliceInt {
            cap: 0,
            len: slice_data.len() as i64,
            ptr: slice_data.as_ptr(),
        };
        let slice_sum = xu_main_xu_sum_slice(&slice.ptr);
        println!("    xu_sum_slice([2, 4, 6, 8, 10]) = {} (expected 30)", slice_sum);
        assert_eq!(slice_sum, 30);

        let bytes_data: [u8; 4] = [10, 20, 30, 40];
        let bytes_slice = XuSliceByte {
            cap: 0,
            len: bytes_data.len() as i64,
            ptr: bytes_data.as_ptr(),
        };
        let bytes_sum = xu_main_xu_sum_byte_slice(&bytes_slice.ptr);
        println!("    xu_sum_byte_slice([10, 20, 30, 40]) = {} (expected 100)", bytes_sum);
        assert_eq!(bytes_sum, 100);

        let floats_data: [f64; 3] = [1.5, 2.5, 3.5];
        let floats_slice = XuSliceF64 {
            cap: 0,
            len: floats_data.len() as i64,
            ptr: floats_data.as_ptr() as *const u64,
        };
        let floats_sum = u64_to_f64(xu_main_xu_sum_f64_slice(&floats_slice.ptr));
        println!("    xu_sum_f64_slice([1.5, 2.5, 3.5]) = {:.4} (expected 7.5000)", floats_sum);
        assert!((floats_sum - 7.5).abs() < 1e-9);

        // 10. array
        println!("\n[10. array] Testing fixed-size array [4]int from Rust...");
        let fixed_arr = XuFixedArray4 {
            e3: 400,
            e2: 300,
            e1: 200,
            e0: 100,
        };
        let fixed_sum = xu_main_xu_sum_fixed_array(&fixed_arr.e0);
        println!("    xu_sum_fixed_array([100, 200, 300, 400]) = {} (expected 1000)", fixed_sum);
        assert_eq!(fixed_sum, 1000);

        // 11. enum
        println!("\n[11. enum] Testing enumeration variants from Rust...");
        let status_tag: i64 = 1; // Pending
        let code = xu_main_xu_status_code(&status_tag);
        println!("    xu_status_code(Pending) = {} (expected 200)", code);
        assert_eq!(code, 200);

        // 12. &mut
        println!("\n[12. &mut] Testing mutable reference mutation from Rust...");
        let mut counter = XuCounter { val: 80 };
        xu_main_xu_mutate_counter(&mut counter.val, 20);
        println!("    xu_mutate_counter(&mut {{80}}, 20) -> val = {} (expected 100)", counter.val);
        assert_eq!(counter.val, 100);

        // 13. callback
        println!("\n[13. callback] Testing Rust extern \"C\" callback passed to Xu...");
        let closure = XuClosure {
            env: std::ptr::null_mut(),
            fn_ptr: rust_mult_callback as *const c_void,
        };
        let cb_res = xu_main_xu_apply_callback(8, 9, &closure.fn_ptr);
        println!("    xu_apply_callback(8, 9, rust_mult_callback) = {} (expected 72)", cb_res);
        assert_eq!(cb_res, 72);

        // 14. void
        println!("\n[14. void] Testing void procedure from Rust:");
        print!("    ");
        xu_main_xu_hello_from_lib();

        println!("\n====================================================================");
        println!("   >>> ALL RUST FFI ASSERTIONS PASSED SUCCESSFULLY! <<<");
        println!("====================================================================\n");
    }
}

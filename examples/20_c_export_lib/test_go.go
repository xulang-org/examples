package main

/*
#cgo CFLAGS: -I.
#cgo LDFLAGS: -L. -lxulib
#include <stdlib.h>
#include "xulib.h"

static int64_t go_c_callback(void *env, int64_t a, int64_t b) {
    (void)env;
    return a * b;
}

static inline int64_t invoke_xu_callback(int64_t a, int64_t b) {
    return xu_apply_callback(a, b, go_c_callback, NULL);
}
*/
import "C"

import (
	"fmt"
	"math"
	"os"
	"unsafe"
)

func main() {
	fmt.Println("====================================================================")
	fmt.Println("   Go (cgo) Host Calling Xu Dynamic Library (libxulib)")
	fmt.Println("====================================================================")
	fmt.Println()

	// 1. int (64-bit integer)
	fmt.Println("[1. int] Testing 64-bit integer arithmetic...")
	sum := int64(C.xu_main_xu_add_int(C.int64_t(2000), C.int64_t(3456)))
	fmt.Printf("    xu_add_int(2000, 3456) = %d (expected 5456)\n", sum)
	if sum != 5456 {
		panic("assert failed: sum != 5456")
	}

	fact := int64(C.xu_main_xu_factorial(C.int64_t(5)))
	fmt.Printf("    xu_factorial(5) = %d (expected 120)\n", fact)
	if fact != 120 {
		panic("assert failed: fact != 120")
	}

	// 2. i32 (32-bit signed integer)
	fmt.Println("\n[2. i32] Testing 32-bit signed integer...")
	sumI32 := int32(C.xu_main_xu_add_i32(C.int64_t(-40), C.int64_t(90)))
	fmt.Printf("    xu_add_i32(-40, 90) = %d (expected 50)\n", sumI32)
	if sumI32 != 50 {
		panic("assert failed: sumI32 != 50")
	}

	clamped := int32(C.xu_main_xu_clamp_i32(C.int64_t(500), C.int64_t(0), C.int64_t(100)))
	fmt.Printf("    xu_clamp_i32(500, 0, 100) = %d (expected 100)\n", clamped)
	if clamped != 100 {
		panic("assert failed: clamped != 100")
	}

	// 3. u32 (32-bit unsigned integer)
	fmt.Println("\n[3. u32] Testing 32-bit unsigned integer...")
	sumU32 := uint32(C.xu_main_xu_add_u32(C.uint32_t(0x10000000), C.uint32_t(0x00234567)))
	fmt.Printf("    xu_add_u32 = 0x%08X (expected 0x10234567)\n", sumU32)
	if sumU32 != 0x10234567 {
		panic("assert failed")
	}

	rot := uint32(C.xu_main_xu_rotate_left_u32(C.uint32_t(1), C.uint32_t(6)))
	fmt.Printf("    xu_rotate_left_u32(1, 6) = %d (expected 64)\n", rot)
	if rot != 64 {
		panic("assert failed")
	}

	// 4. u8 (8-bit byte)
	fmt.Println("\n[4. u8] Testing 8-bit unsigned byte...")
	masked := uint8(C.xu_main_xu_mask_u8(C.uint8_t(0xAA), C.uint8_t(0x0F)))
	fmt.Printf("    xu_mask_u8(0xAA, 0x0F) = 0x%02X (expected 0x0A)\n", masked)
	if masked != 0x0A {
		panic("assert failed")
	}

	sumU8 := uint8(C.xu_main_xu_add_u8(C.uint8_t(150), C.uint8_t(100)))
	fmt.Printf("    xu_add_u8(150, 100) = %d (expected 250)\n", sumU8)
	if sumU8 != 250 {
		panic("assert failed")
	}

	// 5. bool
	fmt.Println("\n[5. bool] Testing boolean logic...")
	even := uint8(C.xu_main_xu_is_even(C.int64_t(500)))
	fmt.Printf("    xu_is_even(500) = %t\n", even != 0)
	if even == 0 {
		panic("assert failed")
	}

	land := uint8(C.xu_main_xu_logical_and(C.uint8_t(1), C.uint8_t(1)))
	fmt.Printf("    xu_logical_and(true, true) = %t\n", land != 0)
	if land == 0 {
		panic("assert failed")
	}

	// 6. f64
	fmt.Println("\n[6. f64] Testing 64-bit IEEE-754 float...")
	fAdd := float64(C.xu_add_f64(C.double(12.25), C.double(7.75)))
	fmt.Printf("    xu_add_f64(12.25, 7.75) = %.4f (expected 20.0000)\n", fAdd)
	if math.Abs(fAdd-20.0) > 1e-9 {
		panic("assert failed")
	}

	scaled := float64(C.xu_scale_f64(C.double(3.5), C.double(4.0)))
	fmt.Printf("    xu_scale_f64(3.5, 4.0) = %.4f (expected 14.0000)\n", scaled)
	if math.Abs(scaled-14.0) > 1e-9 {
		panic("assert failed")
	}

	// 7. string
	fmt.Println("\n[7. string] Testing UTF-8 string passing from Go...")
	goStr := "Greetings from Go cgo host!"
	cStr := C.CString(goStr)
	defer C.free(unsafe.Pointer(cStr))

	strLen := int64(C.xu_get_string_length(cStr))
	fmt.Printf("    xu_get_string_length('%s') = %d (expected %d)\n", goStr, strLen, len(goStr))
	if strLen != int64(len(goStr)) {
		panic("assert failed")
	}

	countG := int64(C.xu_count_char(cStr, C.char('G')))
	fmt.Printf("    xu_count_char('%s', 'G') = %d (expected 2)\n", goStr, countG)
	if countG != 2 {
		panic("assert failed")
	}

	byteAt := int64(C.xu_string_byte_at(cStr, C.int64_t(0)))
	fmt.Printf("    xu_string_byte_at(0) = %d ('%c', expected 'G')\n", byteAt, rune(byteAt))
	if byteAt != 'G' {
		panic("assert failed")
	}

	fmt.Println("    Calling xu_print_string from Go:")
	fmt.Print("    ")
	C.xu_print_string(cStr)

	// 8. struct
	fmt.Println("\n[8. struct] Testing struct Point2D and MixedRecord from Go...")
	distSq := int64(C.xu_point_distance_sq(C.int64_t(10), C.int64_t(24)))
	fmt.Printf("    xu_point_distance_sq(Point2D(10, 24)) = %d (expected 676)\n", distSq)
	if distSq != 676 {
		panic("assert failed")
	}

	checkRec := bool(C.xu_check_mixed_record(C.int64_t(606), C.double(95.0), C.bool(true)))
	fmt.Printf("    xu_check_mixed_record(606, 95.0, true) = %t\n", checkRec)
	if !checkRec {
		panic("assert failed")
	}

	// 9. slice
	fmt.Println("\n[9. slice] Testing slice views ([]int, []u8, []f64) from Go...")
	sliceData := []int64{100, 200, 300, 400}
	sliceSum := int64(C.xu_sum_slice((*C.int64_t)(&sliceData[0]), C.int64_t(len(sliceData))))
	fmt.Printf("    xu_sum_slice([100, 200, 300, 400]) = %d (expected 1000)\n", sliceSum)
	if sliceSum != 1000 {
		panic("assert failed")
	}

	byteData := []uint8{2, 4, 6, 8}
	byteSum := int64(C.xu_sum_byte_slice((*C.uint8_t)(&byteData[0]), C.int64_t(len(byteData))))
	fmt.Printf("    xu_sum_byte_slice([2, 4, 6, 8]) = %d (expected 20)\n", byteSum)
	if byteSum != 20 {
		panic("assert failed")
	}

	floatData := []float64{1.2, 2.3, 3.5}
	floatSum := float64(C.xu_sum_f64_slice((*C.double)(&floatData[0]), C.int64_t(len(floatData))))
	fmt.Printf("    xu_sum_f64_slice([1.2, 2.3, 3.5]) = %.4f (expected 7.0000)\n", floatSum)
	if math.Abs(floatSum-7.0) > 1e-9 {
		panic("assert failed")
	}

	// 10. array ([4]int)
	fmt.Println("\n[10. array] Testing fixed-size array [4]int from Go...")
	arrSum := int64(C.xu_sum_fixed_array(C.int64_t(10), C.int64_t(20), C.int64_t(30), C.int64_t(40)))
	fmt.Printf("    xu_sum_fixed_array([10, 20, 30, 40]) = %d (expected 100)\n", arrSum)
	if arrSum != 100 {
		panic("assert failed")
	}

	// 11. enum
	fmt.Println("\n[11. enum] Testing enumeration variants from Go...")
	codePending := int64(C.xu_status_code(C.XU_STATUS_PENDING))
	fmt.Printf("    xu_status_code(Pending) = %d (expected 200)\n", codePending)
	if codePending != 200 {
		panic("assert failed")
	}

	// 12. $&
	fmt.Println("\n[12. $&] Testing mutable struct pointer from Go...")
	var cnt C.XuCounter
	cnt.val = C.int64_t(250)
	C.xu_mutate_counter(&cnt, C.int64_t(50))
	fmt.Printf("    xu_mutate_counter(&cnt{250}, 50) -> cnt.val = %d (expected 300)\n", int64(cnt.val))
	if int64(cnt.val) != 300 {
		panic("assert failed")
	}

	// 13. callback
	fmt.Println("\n[13. callback] Testing Go/C callback passed to Xu...")
	cbRes := int64(C.invoke_xu_callback(C.int64_t(11), C.int64_t(12)))
	fmt.Printf("    xu_apply_callback(11, 12, callback) = %d (expected 132)\n", cbRes)
	if cbRes != 132 {
		panic("assert failed")
	}

	// 14. void
	fmt.Println("\n[14. void] Testing void procedure from Go:")
	fmt.Print("    ")
	C.xu_main_xu_hello_from_lib()

	fmt.Println("\n====================================================================")
	fmt.Println("   >>> ALL GO (CGO) ASSERTIONS PASSED SUCCESSFULLY! <<<")
	fmt.Println("====================================================================")
	fmt.Println()
	_ = os.Stdout
}

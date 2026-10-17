#include "xu_native.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*
 * Xu <-> C 跨语言原生调用桥接示例 (xu-c-1 ABI)
 * 演示多数据类型的双向互操作规范与参数/返回值传递机制
 */

/*
 * 1. 64 位有符号整数 (int <-> xu_int / int64_t)
 * 常用于常规数值计算、索引偏移与 64 位整型参数。
 */
xu_int XU_NATIVE_SYMBOL(xu_native_add_add)(xu_int a, xu_int b) {
    return a + b;
}

/*
 * 2. 64 位双精度浮点数 (f64 <-> double)
 * Xu 将 64 位标量浮点位模式（IEEE-754）直接传至 ABI 通用寄存器。
 * C 侧通过位转换辅助函数与标准 double 之间无损映射。
 */
static inline double xu_to_double(xu_int bits) {
    double val;
    memcpy(&val, &bits, sizeof(double));
    return val;
}

static inline xu_int double_to_xu(double val) {
    xu_int bits;
    memcpy(&bits, &val, sizeof(xu_int));
    return bits;
}

xu_int XU_NATIVE_SYMBOL(xu_native_add_scale_f64)(xu_int val_bits, xu_int factor_bits) {
    double v = xu_to_double(val_bits);
    double f = xu_to_double(factor_bits);
    double res = v * f;
    return double_to_xu(res);
}

/*
 * 3. 布尔类型 (bool <-> uint8_t)
 * 返回 1 代表 true，0 代表 false。
 */
uint8_t XU_NATIVE_SYMBOL(xu_native_add_is_even)(xu_int n) {
    return (n % 2 == 0) ? 1 : 0;
}

/*
 * 4. 32 位有符号整数 (i32 <-> int32_t)
 * 常用于 32 位系统 API、错误码和边界限制。
 */
int32_t XU_NATIVE_SYMBOL(xu_native_add_clamp_i32)(int32_t val, int32_t min_val, int32_t max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

/*
 * 5. 32 位无符号整数 (u32 <-> uint32_t)
 * 常用于位掩码、位移、哈希与校验和计算。
 */
uint32_t XU_NATIVE_SYMBOL(xu_native_add_rotate_left_u32)(uint32_t val, uint32_t shift) {
    shift &= 31;
    if (shift == 0) return val;
    return (val << shift) | (val >> (32 - shift));
}

/*
 * 6. 8 位无符号字节 (u8 <-> uint8_t)
 * 常用于单字节协议分析、字符码与像素通道处理。
 */
uint8_t XU_NATIVE_SYMBOL(xu_native_add_mask_u8)(uint8_t b, uint8_t mask) {
    return (uint8_t)(b & mask);
}

/*
 * 7. 无返回值过程与状态副作用 (void <-> void)
 * 演示 C 状态维护（计数器、句柄表、资源池初始化等）。
 */
static xu_int g_counter = 0;

void XU_NATIVE_SYMBOL(xu_native_add_reset_counter)(void) {
    g_counter = 0;
}

void XU_NATIVE_SYMBOL(xu_native_add_add_to_counter)(xu_int delta) {
    g_counter += delta;
}

xu_int XU_NATIVE_SYMBOL(xu_native_add_get_counter)(void) {
    return g_counter;
}

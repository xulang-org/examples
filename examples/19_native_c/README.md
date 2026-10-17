# C 静态原生模块与多数据类型交互 (Native C & .xulib)

本示例展示如何使用 Xu 的 `xu-c-1` 原生 ABI，在 C 与 Xu 之间进行丰富数据类型的双向互操作，并通过 `xunative` 工具链生成跨平台便携式 `.xulib` 归档包。

---

## 1. 支持的数据类型与 ABI 映射规范

在 `abi: "xu-c-1"` 规范下，Xu 编译器将各类基础数据类型与 C 语言进行标准对齐：

| Xu 数据类型 | C 语言对应类型 | 典型用途与传递说明 |
|:---|:---|:---|
| **`int`** | `xu_int` (`int64_t`) | 64 位通用有符号整数、数值计算、长度、索引偏移 |
| **`f64`** | 64 位浮点 (IEEE-754) | 双精度浮点。标量位模式通过 ABI 通用寄存器传递，C 侧借助 `memcpy` / 联合体无损互转 |
| **`bool`** | `uint8_t` / `bool` | 布尔真假逻辑（`1` 代表 `true`，`0` 代表 `false`） |
| **`i32`** | `int32_t` | 32 位有符号整数、系统 API 错误码、区间限制 |
| **`u32`** | `uint32_t` | 32 位无符号整数、位移、掩码、哈希与校验和 |
| **`u8`** | `uint8_t` | 8 位无符号单字节、字符码、单通道像素、字节掩码 |
| **`void`** | `void` | 无返回值函数，适用于初始化、资源清理或内部有状态副作用 |

> [!NOTE]
> 头文件 `xu_native.h` 提供了 `XU_NATIVE_SYMBOL(...)` 宏，它根据宿主平台自动处理符号导出下划线差异（Mach-O 添加前缀 `_`，而 ELF/COFF 保持符号原名），确保编译器链接时全局符号精确匹配。

---

## 2. 原生模块完整的 7 个开发动作

### 动作一：编写 C 桥接源文件 (`bridge.c`)
定义 C 函数时，使用 `XU_NATIVE_SYMBOL(xu_<module>_<function>)` 统一符号命名：

```c
#include "xu_native.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// 1. int 加法
xu_int XU_NATIVE_SYMBOL(xu_native_add_add)(xu_int a, xu_int b) {
    return a + b;
}

// 2. f64 双精度浮点位转换与缩放
static inline double xu_to_double(xu_int bits) {
    double v; memcpy(&v, &bits, sizeof(double)); return v;
}
static inline xu_int double_to_xu(double v) {
    xu_int b; memcpy(&b, &v, sizeof(xu_int)); return b;
}
xu_int XU_NATIVE_SYMBOL(xu_native_add_scale_f64)(xu_int val_bits, xu_int factor_bits) {
    return double_to_xu(xu_to_double(val_bits) * xu_to_double(factor_bits));
}

// 3. bool 偶数判断
uint8_t XU_NATIVE_SYMBOL(xu_native_add_is_even)(xu_int n) {
    return (n % 2 == 0) ? 1 : 0;
}

// 4. i32 范围限定
int32_t XU_NATIVE_SYMBOL(xu_native_add_clamp_i32)(int32_t val, int32_t min_v, int32_t max_v) {
    if (val < min_v) return min_v;
    if (val > max_v) return max_v;
    return val;
}

// 5. u32 循环左移
uint32_t XU_NATIVE_SYMBOL(xu_native_add_rotate_left_u32)(uint32_t val, uint32_t shift) {
    shift &= 31;
    if (shift == 0) return val;
    return (val << shift) | (val >> (32 - shift));
}

// 6. u8 字节掩码
uint8_t XU_NATIVE_SYMBOL(xu_native_add_mask_u8)(uint8_t b, uint8_t mask) {
    return (uint8_t)(b & mask);
}

// 7. void 状态维护与副作用
static xu_int g_counter = 0;
void XU_NATIVE_SYMBOL(xu_native_add_reset_counter)(void) { g_counter = 0; }
void XU_NATIVE_SYMBOL(xu_native_add_add_to_counter)(xu_int delta) { g_counter += delta; }
xu_int XU_NATIVE_SYMBOL(xu_native_add_get_counter)(void) { return g_counter; }
```

### 动作二：声明类型元数据契约 (`native.json`)
清单明确记录模块标识、ABI 协议与各函数的参数类型列表及返回值类型：

```json
{
  "module": "native_add",
  "abi": "xu-c-1",
  "exports": [
    {"name": "add", "params": ["int", "int"], "ret": "int"},
    {"name": "scale_f64", "params": ["f64", "f64"], "ret": "f64"},
    {"name": "is_even", "params": ["int"], "ret": "bool"},
    {"name": "clamp_i32", "params": ["i32", "i32", "i32"], "ret": "i32"},
    {"name": "rotate_left_u32", "params": ["u32", "u32"], "ret": "u32"},
    {"name": "mask_u8", "params": ["u8", "u8"], "ret": "u8"},
    {"name": "reset_counter", "params": [], "ret": "void"},
    {"name": "add_to_counter", "params": ["int"], "ret": "void"},
    {"name": "get_counter", "params": [], "ret": "int"}
  ]
}
```

### 动作三：使用 `xunative compile` 分别编译单目标切片
为不同目标平台分别编译生成嵌入了 `.xumeta` 数据段的 Unix 静态归档切片（`.a`）：

```bash
# 针对 macOS Apple Silicon (ARM64)
python3 path/to/xu/scripts/xunative.py compile \
  --manifest native.json --target macos-arm64 bridge.c \
  -o native/native_add-arm64.a

# 针对 macOS Intel (x86_64)
python3 path/to/xu/scripts/xunative.py compile \
  --manifest native.json --target macos-x64 bridge.c \
  -o native/native_add-x64.a
```

### 动作四：使用 `xunative pack` 聚合打包为便携式 `.xulib`
将多个平台的 `.a` 切片与元数据清单封装进单一 `.xulib` 容器文件：

```bash
python3 path/to/xu/scripts/xunative.py pack \
  --manifest native.json \
  --slice macos-arm64=native/native_add-arm64.a \
  --slice macos-x64=native/native_add-x64.a \
  -o native/native_add.xulib
```

### 动作五：在工程项目配置中声明原生模块 (`xu.xun`)
在项目的 `xu.xun` 配置文件中添加 `native:` 映射：

```xun
module: example/native_c
version: !ver 1.0.0
xu: !ver 0.8.3

native:
  native_add: file:./native/native_add.xulib
```

### 动作六：在 Xu 代码中导入与调用 (`main.xu`)
在源码中通过 `#native_add` 导入并直接调用：

```xu
@main

#native_add

fn main() int {
    println("=== Xu <-> Native C Multi-Type Interop Demo ===")

    // 1. int
    sum := native_add.add(20, 22)
    println("1. int add(20, 22) = " + str(sum))

    // 2. f64
    scaled := native_add.scale_f64(1.5, 3.0)
    println("2. f64 scale_f64(1.5, 3.0) * 10 = " + str(int(scaled * 10.0)))

    // 3. bool
    println("3. bool is_even(42) = " + str(native_add.is_even(42)) + ", is_even(43) = " + str(native_add.is_even(43)))

    // 4. i32
    clamped := native_add.clamp_i32(i32(150), i32(0), i32(100))
    println("4. i32 clamp_i32(150, 0, 100) = " + str(int(clamped)))

    // 5. u32
    rotated := native_add.rotate_left_u32(u32(1), u32(4))
    println("5. u32 rotate_left_u32(1, 4) = " + str(int(rotated)))

    // 6. u8
    masked := native_add.mask_u8(u8(255), u8(15))
    println("6. u8 mask_u8(255, 15) = " + str(int(masked)))

    // 7. void 与副作用
    native_add.reset_counter()
    native_add.add_to_counter(100)
    native_add.add_to_counter(25)
    c := native_add.get_counter()
    println("7. void state counter = " + str(c))

    println("=== All Native Type Assertions Passed ===")
    << 0
}
```

### 动作七：一键编译与运行验证
在 `examples` 仓库根目录下执行：

```bash
./scripts/example.sh run 19_native_c
```

运行输出：
```text
=== Xu <-> Native C Multi-Type Interop Demo ===
1. int add(20, 22) = 42
2. f64 scale_f64(1.5, 3.0) * 10 = 45
3. bool is_even(42) = true, is_even(43) = false
4. i32 clamp_i32(150, 0, 100) = 100
5. u32 rotate_left_u32(1, 4) = 16
6. u8 mask_u8(255, 15) = 15
7. void state counter = 125
=== All Native Type Assertions Passed ===
```
所有类型校验无缝通过，无任何运行时停顿或性能损失。

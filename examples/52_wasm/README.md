# Xu 编译到 WebAssembly

使用 Xu 0.8.6 源码中的实验性 `wasm32` 后端，将加法、循环、递归直接编译成
WASM。`main.xu` 保存计算逻辑，JavaScript 只传递参数和读取结果。
需要已构建、支持 `wasm32` 的 Xu 产品编译器；历史 0.8.3 SDK 不支持这个目标。

在 examples 仓库根执行：

```sh
make check EXAMPLE=52_wasm
make build EXAMPLE=52_wasm
make run EXAMPLE=52_wasm # Node.js 校验并打印结果
# 非默认编译器：XU=/path/to/xu XU_ROOT=/path/to/matching/xu make run EXAMPLE=52_wasm
```

也可以直接编译，无须 Rust、LLVM 或 Emscripten：

```sh
../xu/bin/xu build examples/52_wasm --target wasm32 -O0 -o build/52_wasm.wasm
node examples/52_wasm/run.mjs build/52_wasm.wasm
```

预期结果：`add(20, 22) = 42`、`fibonacci(10) = 55`、`sum_to(100) = 5050`。
Node.js 示例还验证负数和 `main` 返回值。

浏览器运行：

```sh
python3 -m http.server 8080 --bind 127.0.0.1
# 打开 http://127.0.0.1:8080/examples/52_wasm/
```

产物导出 `memory`、`main_frame` 和公开函数的 `<name>_frame`。
每个函数接受 `(argument_frame: i32, return_frame: i32)`，参数按 8 字节槽位向低地址
排列，结果同样向低地址排列。Xu 的 `int` 为 i64；JavaScript 用 `BigInt` 与
`DataView.setBigInt64/getBigInt64(..., true)` 读写。示例使用参数地址 800 和结果地址 768。

当前目标使用自定义 frame ABI，未接入 WASI。涉及分配、字符串拼接、打印的程序需要
提供对应 Xu 运行时；官网运行器提供这些支持。本例只使用无需运行时分配的整数计算。
原生线程、文件/网络和 f64 运算尚不支持，不能把所有原生 Xu 程序直接改目标后运行。
生成的 WASM 放在忽略的 `build/` 中。

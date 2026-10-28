# 01_hello

<p align="center"><img src="../../brand/xu_classic.svg" alt="Xu" width="64" height="64"></p>

最小程序：打印一行并退出。

## 本示例体现的特色

- 入口 `fn main() int`，退出码由 `<< 0` 返回（无 `return` 关键字）
- 无分号、极简骨架

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
$XU run examples/01_hello
# 或分步：
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/01_hello/main.xu -o /tmp/xu_01_hello
chmod +x /tmp/xu_01_hello && /tmp/xu_01_hello
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/01_hello/main.xu
```

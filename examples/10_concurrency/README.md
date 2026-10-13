# 10_concurrency

`xu` 启动协程与带缓冲 `chan` 收发。

## 本示例体现的特色

- **`xu f(args)` 启动协程**
- `xu.pkg.f` 带点号是包路径调用，不是 spawn
- `chan` 转移所有权；不把借用送进协程
- 与类型系统联动，数据竞争尽量编译期拒绝

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/10_concurrency/main.xu -o /tmp/xu_10_concurrency
chmod +x /tmp/xu_10_concurrency && /tmp/xu_10_concurrency
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/10_concurrency/main.xu
```

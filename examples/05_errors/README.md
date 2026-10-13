# 05_errors

`>>` 抛出与 `or (err) { … }` 接住；失败给默认值用返回 `string` 的包装函数。

## 本示例体现的特色

- **不要** `Result` / `Ok` / `Err` / `?`
- `>> errors.new(...)` 抛出；调用处 `or (err) { … }` 强制接住
- `#errors` 前缀导入（不是 `import "..."`）

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/05_errors/main.xu -o /tmp/xu_05_errors
chmod +x /tmp/xu_05_errors && /tmp/xu_05_errors
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/05_errors/main.xu
```

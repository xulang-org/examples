# 04_functions_methods

普通函数、struct、值方法与 `&mut` 方法。

## 本示例体现的特色

- 无 class：`struct` + 方法
- 接收者类型写 `T` / `&T` / `&mut T`（不是 `fn (mut c T)`）
- 成功路径用 `<<` 返回

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/04_functions_methods/main.xu -o /tmp/xu_04_functions_methods
chmod +x /tmp/xu_04_functions_methods && /tmp/xu_04_functions_methods
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/04_functions_methods/main.xu
```

# 07_arrays_strings

数组 `push` / `for-in`、字符串长度与切片、`$ident` 插值。

## 本示例体现的特色

- 动态数组 `[]T` + `push`
- 字符串切片 **拷贝语义**（无 GC 下不共享视图）
- `"hi $name"` 插值

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/07_arrays_strings/main.xu -o /tmp/xu_07_arrays_strings
chmod +x /tmp/xu_07_arrays_strings && /tmp/xu_07_arrays_strings
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/07_arrays_strings/main.xu
```

# 09_generics_temp

`struct@` 方法集、`item# Bound` 静态单态、`fn f#T`、裸 `temp` 的 `is` / `as`。

## 本示例体现的特色

- **`temp` 一词根**：裸 `temp` 动态盒；`#` 值泛型；`@` 方法集约束
- 默认静态单态（零虚表主路径）；有方法即满足，无需 `impl`
- `is` 判断不销毁；`as T` 传值并销毁盒子

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/09_generics_temp/main.xu -o /tmp/xu_09_generics_temp
chmod +x /tmp/xu_09_generics_temp && /tmp/xu_09_generics_temp
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/09_generics_temp/main.xu
```

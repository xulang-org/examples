# 02_bindings

不可变绑定、`$`、`const`。

## 本示例体现的特色

- **默认不可变**：`name := "..."` 不能改写
- 改写必须显式 `$`；`const` 为编译期常量
- 无全局可变状态

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/02_bindings/main.xu -o /tmp/xu_02_bindings
chmod +x /tmp/xu_02_bindings && /tmp/xu_02_bindings
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/02_bindings/main.xu
```

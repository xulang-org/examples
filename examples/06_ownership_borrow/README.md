# 06_ownership_borrow

按值 move、`&T` 共享借用、`$&T` 独占借用。

## 本示例体现的特色

- **无 GC**：按值传 `string` 即 move
- 共享借用 `&x`；独占借用 `$&x`（绑定须 `$`）
- 安全边界尽量在编译期关闭

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/06_ownership_borrow/main.xu -o /tmp/xu_06_ownership_borrow
chmod +x /tmp/xu_06_ownership_borrow && /tmp/xu_06_ownership_borrow
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/06_ownership_borrow/main.xu
```

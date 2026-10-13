# 08_enum_match

`enum` 单元/元组变体与穷尽 `match`。

## 本示例体现的特色

- 标签联合 `enum`，构造 `Color.Rgb(1, 2, 3)`
- `match` 须穷尽（或带 `else`）
- 不用语言面 `Option`/`Result` 套件做错误（错误走 `>>`/`or`）

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/08_enum_match/main.xu -o /tmp/xu_08_enum_match
chmod +x /tmp/xu_08_enum_match && /tmp/xu_08_enum_match
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/08_enum_match/main.xu
```

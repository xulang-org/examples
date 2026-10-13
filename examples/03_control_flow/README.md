# 03_control_flow

`if` / `match` / `while` / `for`，以及语句首 `->`（continue）、`<-`（break）。

## 本示例体现的特色

- 条件 / 循环**无括号**
- 区间：`0:n`（半开 `[0, n)`）；不是 `0..n` / `0 -> n`
- 控制流出口是符号：`->` continue、`<-` break（不是关键字）

详见 [`FEATURES.md`](../../FEATURES.md)。

## 编译与运行

在仓库根：

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/03_control_flow/main.xu -o /tmp/xu_03_control_flow
chmod +x /tmp/xu_03_control_flow && /tmp/xu_03_control_flow
```

仅检查：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" check examples/03_control_flow/main.xu
```

# 11_http_server

自己 `listen` / `accept` / 按 path 分支。静态目录见 [`12_file_server`](../12_file_server)；`Handler` 上下文见 [`13_http_context`](../13_http_context)。

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/11_http_server/main.xu -o /tmp/xu_11_http
/tmp/xu_11_http
# http://127.0.0.1:18080
# GET / /hello /health
# POST /echo
```

如需排查 primer，只能显式走 oracle/recovery（不属于产品或自举证据）：

```bash
cargo build --manifest-path bootstrap/Cargo.toml -p xu
XU_BOOT=./bootstrap/target/debug/xu
"$XU_BOOT" examples/11_http_server/main.xu -o /tmp/xu_11_http_oracle
```

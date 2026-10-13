# 13_http_context

 的 `Handler` / `HandlerFunc`：`func(w, r)`，Xu 里是 `http.Context`。

```go
func hello(w http.ResponseWriter, r *http.Request) {
 fmt.Fprint(w, "hello")
}
mux := http.NewServeMux()
mux.HandleFunc("/hello", hello)
http.ListenAndServe(":8080", mux)
```

```xu
fn hello(c http.Context) {
 c.text(200, "hello")
}
http.listen_and_serve_handler("127.0.0.1:18081", hello)
```

一个 `app(c)` 里按 `c.path()` 分支即可（ 的 `switch r.URL.Path`）。

另外两种：[`11_http_server`](../11_http_server) 自己 accept；[`12_file_server`](../12_file_server) FileServer。

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/13_http_context/main.xu -o /tmp/xu_13_ctx
/tmp/xu_13_ctx
# http://127.0.0.1:18081
```

如需排查 primer，只能显式走 oracle/recovery（不属于产品或自举证据）：

```bash
cargo build --manifest-path bootstrap/Cargo.toml -p xu
XU_BOOT=./bootstrap/target/debug/xu
"$XU_BOOT" examples/13_http_context/main.xu -o /tmp/xu_13_ctx_oracle
```

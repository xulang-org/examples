# 12_file_server

<p align="center"><img src="../../../brand/xu_classic.svg" alt="Xu" width="64" height="64"></p>

 的 `FileServer` 一行。自己 accept 见 [`11_http_server`](../11_http_server)；`Handler` 上下文见 [`13_http_context`](../13_http_context)。

```go
http.ListenAndServe(":8080", http.FileServer(http.Dir(".")))
```

```xu
http.listen_and_serve("127.0.0.1:8080", ".")
```

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/12_file_server/main.xu -o /tmp/xu_12_fs
/tmp/xu_12_fs
# 默认根目录：examples/12_file_server/www
# 或：/tmp/xu_12_fs /path/to/dir
# http://127.0.0.1:8080
```

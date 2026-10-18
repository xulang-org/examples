# 14_tcp_echo

 的 `net.Listen("tcp", …)` + `Accept` + 每连接一个 goroutine。Xu 里是 `listen_on` / `accept` / `xu handle`。

```go
ln, _ := net.Listen("tcp", "127.0.0.1:19000")
for {
 c, _ := ln.Accept()
 go func(c net.Conn) {
 buf := make([]byte, 4096)
 n, _ := c.Read(buf)
 c.Write(buf[:n])
 c.Close()
 }(c)
}
```

```xu
ln := net.listen_on("tcp", "127.0.0.1:19000") or (err) { << 1 }
c := ln.accept() or (err) { -> }
xu handle(c)
```

UDP 数据报见 [`15_udp_echo`](../15_udp_echo)。HTTP 见 [`11_http_server`](../11_http_server)。

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/14_tcp_echo/main.xu -o /tmp/xu_14_tcp
/tmp/xu_14_tcp
# 另一终端：printf ping | nc 127.0.0.1 19000
```

# 15_udp_echo

 的 `net.ListenPacket("udp", …)` / `ListenUDP`：读一个 datagram，原样写回对端。

```go
c, _ := net.ListenPacket("udp", "127.0.0.1:19001")
buf := make([]byte, 4096)
n, addr, _ := c.ReadFrom(buf)
c.WriteTo(buf[:n], addr)
```

```xu
mut c := net.listen_packet("udp", "127.0.0.1:19001") or (err) { << 1 }
pkt := c.read_from(4096) or (err) { -> }
c.write_to(&pkt.data, &pkt.addr)
```

TCP 流见 [`14_tcp_echo`](../14_tcp_echo)。HTTP 见 [`11_http_server`](../11_http_server)。

```bash
XU="$(pwd)/../xu/bin/xu" # 当前产品编译器
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/15_udp_echo/main.xu -o /tmp/xu_15_udp
/tmp/xu_15_udp
# 另一终端：printf ping | nc -u -w1 127.0.0.1 19001
```

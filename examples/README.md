# 示例索引

<p align="center"><img src="../brand/xu_classic.svg" alt="Xu" width="72" height="72"></p>

前 01–16 个示例与 [`USAGE.md`](../USAGE.md) 各节、[`FEATURES.md`](../FEATURES.md) 特色表一一对应；17–18 是标准库数据库附加示例，19 展示 C/XUNATIVE 静态模块。每个目录含 `main.xu` 与带**特色说明 + 编译命令**的 `README.md`。

在仓库根先选定编译器（任选其一）：

```bash
XU="$(pwd)/../xu/bin/xu" # 与当前 src/ 配套的产品编译器
# 历史快照 developer/bin/xu.* 仅用于对应版本源码
```

通用模板：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/<目录>/main.xu -o /tmp/xu_ex
chmod +x /tmp/xu_ex && /tmp/xu_ex
```

| 目录 | 对应帮助章节 | 编译命令（仓库根） |
|------|----------------|-------------------|
| [`01_hello`](01_hello) | §1 程序骨架 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/01_hello/main.xu -o /tmp/xu_01_hello && chmod +x /tmp/xu_01_hello && /tmp/xu_01_hello` |
| [`02_bindings`](02_bindings) | §2 绑定与常量 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/02_bindings/main.xu -o /tmp/xu_02_bindings && chmod +x /tmp/xu_02_bindings && /tmp/xu_02_bindings` |
| [`03_control_flow`](03_control_flow) | §4 控制流 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/03_control_flow/main.xu -o /tmp/xu_03_control_flow && chmod +x /tmp/xu_03_control_flow && /tmp/xu_03_control_flow` |
| [`04_functions_methods`](04_functions_methods) | §5 函数与方法 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/04_functions_methods/main.xu -o /tmp/xu_04_functions_methods && chmod +x /tmp/xu_04_functions_methods && /tmp/xu_04_functions_methods` |
| [`05_errors`](05_errors) | §6 错误处理 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/05_errors/main.xu -o /tmp/xu_05_errors && chmod +x /tmp/xu_05_errors && /tmp/xu_05_errors` |
| [`06_ownership_borrow`](06_ownership_borrow) | §7 所有权与借用 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/06_ownership_borrow/main.xu -o /tmp/xu_06_ownership_borrow && chmod +x /tmp/xu_06_ownership_borrow && /tmp/xu_06_ownership_borrow` |
| [`07_arrays_strings`](07_arrays_strings) | §8 数组与字符串 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/07_arrays_strings/main.xu -o /tmp/xu_07_arrays_strings && chmod +x /tmp/xu_07_arrays_strings && /tmp/xu_07_arrays_strings` |
| [`08_enum_match`](08_enum_match) | §9 Enum | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/08_enum_match/main.xu -o /tmp/xu_08_enum_match && chmod +x /tmp/xu_08_enum_match && /tmp/xu_08_enum_match` |
| [`09_generics_temp`](09_generics_temp) | §10 泛型 / any | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/09_generics_temp/main.xu -o /tmp/xu_09_generics_temp && chmod +x /tmp/xu_09_generics_temp && /tmp/xu_09_generics_temp` |
| [`10_concurrency`](10_concurrency) | §11 并发 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/10_concurrency/main.xu -o /tmp/xu_10_concurrency && chmod +x /tmp/xu_10_concurrency && /tmp/xu_10_concurrency` |
| [`11_http_server`](11_http_server) | HTTP 自己路由 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/11_http_server/main.xu -o /tmp/xu_11_http && /tmp/xu_11_http` |
| [`12_file_server`](12_file_server) | HTTP FileServer | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/12_file_server/main.xu -o /tmp/xu_12_fs && /tmp/xu_12_fs` |
| [`13_http_context`](13_http_context) | HTTP Context / mux | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/13_http_context/main.xu -o /tmp/xu_13_ctx && /tmp/xu_13_ctx` |
| [`14_tcp_echo`](14_tcp_echo) | TCP echo | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/14_tcp_echo/main.xu -o /tmp/xu_14_tcp && /tmp/xu_14_tcp` |
| [`15_udp_echo`](15_udp_echo) | UDP echo | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/15_udp_echo/main.xu -o /tmp/xu_15_udp && /tmp/xu_15_udp` |
| [`16_ui_webview`](16_ui_webview) | UI + WebView 桌面应用（`App/main.xu`+`www/`+`brand/`） | `xu build examples/16_ui_webview/open --target macos-arm64 --desktop -o /tmp/xu_open && /tmp/xu_open` |
| [`16_ui_webview/xu_design_system`](16_ui_webview/xu_design_system) | 设计系统静态 HTTP 服务 | `../xu/scripts/deploy_xu_server.sh build-local --source examples/16_ui_webview/xu_design_system/server.xu` |
| [`17_database_postgres`](17_database_postgres) | PostgreSQL：连接、查询、Prepare、事务 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/17_database_postgres/main.xu -o /tmp/xu_17_postgres && chmod +x /tmp/xu_17_postgres && /tmp/xu_17_postgres` |
| [`18_database_mysql`](18_database_mysql) | MySQL/MariaDB：连接、查询、Prepare、事务 | `../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/18_database_mysql/main.xu -o /tmp/xu_18_mysql && chmod +x /tmp/xu_18_mysql && /tmp/xu_18_mysql` |
| [`19_native_c`](19_native_c) | C→XUNATIVE archive→Xu XUOBJ→系统静态链接 | 见目录 README；macOS arm64 实例输出 `42` |
| [`20_c_export_lib`](20_c_export_lib) | Xu 代码编译为动态库 (.dylib/.so/.dll) 与静态库 (.a/.lib)，由 C/C++/Rust/Python/Go/C# 调用 | 见目录 README；全覆盖 14 种数据类型，跨平台支持 macOS/Linux/Windows |
| [`52_wasm`](52_wasm) | Xu → WASM：公开函数、整数参数、循环、递归 | `make wasm`；浏览器入口见目录 README |

交叉编译示例程序（可选）：

```bash
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/01_hello/main.xu --target linux-x64 -o /tmp/hello_linux_x64
```

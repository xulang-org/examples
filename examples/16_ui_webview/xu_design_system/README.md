# XU VIS & Design System HTTP Server

本目录包含 XU 设计系统的静态资源和一个最小 HTTP/1.1 文件服务。服务默认只监听回环地址，适合本地预览；部署时用 systemd 等进程管理器运行服务，使用绝对 `--root` 路径，保持回环监听，再由 Nginx/Caddy 反向代理到 `127.0.0.1:8088` 并终止 TLS。下方提供本地构建和启动命令。

## 内容入口与维护约定

- `www/index.html`：VIS、UI 组件与 XUFLOW 的主展厅；
- `www/brand/VI_MANUAL.md`：v2.7 法定视觉规范源文件；
- `www/office_merch.html`：22 项实体物料与生产参数展厅；
- `www/vi-ui-design-system.css` / `www/app.js`：主展厅样式与交互；
- HTTP 服务只发布 `www/`，这是完整的预览与交付入口；修改与部署均以该目录为准。目录根部的旧静态副本缺少字体和页面资源，不作为独立预览入口。

## 构建和运行

在仓库根目录执行，需已将 Xu 编译器 `xu` 加入 `PATH`：

```bash
mkdir -p build
xu build examples/16_ui_webview/xu_design_system/server.xu -o build/xu_design_server
./build/xu_design_server --addr 127.0.0.1:8088 \
 --root examples/16_ui_webview/xu_design_system/www
```

本目录的 `server.xu` 是 HTTP 服务入口，`main.xu` 是另一个桌面 WebView 入口。上面的命令显式选择 `server.xu`，避免将两个 `fn main()` 一起按目录构建；静态资源目录无需单独编译。本例未注册到 `scripts/ui_examples.py`。

也可以从仓库根使用统一部署脚本；它在 Xu 仓库内执行编译，因此传入源文件的绝对路径：

```bash
../xu/scripts/deploy_xu_server.sh build-local \
 --source "$PWD/examples/16_ui_webview/xu_design_system/server.xu" \
 --output /tmp/xu_design_server
```

`--help`、`--addr HOST:PORT` 和 `--root DIR` 可用于查看帮助、改变监听地址和静态根目录；旧的 `PORT DIR` 位置参数仍兼容。启动命令应从仓库根执行，或传入绝对根目录。

## 运行约束

- 仅 HTTP/1.1 GET/HEAD 和目录索引，不提供 TLS、认证、限流或业务 readiness。
- 默认 `127.0.0.1:8088`，不要直接暴露到公网。
- Linux 当前串行处理连接；这是 stackful worker 修复前的可靠性回退，不是高并发承诺。
- 目录服务会拒绝 `..` 路径逃逸；静态资源更新后重启进程即可生效。
- `xu_design_server.xin` 等 `.xin` 是本机生成物，不是跨平台发布基线；目标 Linux 请用 `--target linux-x64` 重新构建并核对 SHA-256。

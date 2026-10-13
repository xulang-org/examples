# 16 — UI + WebView 桌面应用示例

<p align="center"><img src="../../../brand/xu_classic.svg" alt="Xu" width="72" height="72"></p>

对应 U 轨：`#ui` 窗口/菜单/托盘 + `#ui/webview` 内容槽。

**分层**： bootstrap 只编译 Xu；打包用产品 xu：`xu doctor` / `xu build <App> --desktop`。

## 桌面应用目录约定（固定）

每个应用是独立项目根：

```text
App/
 xu.xun # 必需：module / require 包映射
 main.xu # 入口（必需）
 www/ # 本地 xu:// 静态页（有 #ui/webview 时；可选）
 index.html
 brand/ # 品牌资源（图标必需）
 xu_app_logo.png # 桌面图标 / 异形蒙版（亦认 brand/icon.png、xu_app_icon.png）
 xu_classic.svg # 内容标识（页面 / favicon）
```

**必需**：`App/brand/` 下至少有一个 PNG 图标（`icon.png` / `xu_app_icon.png` / `xu_app_logo.png`）。pack **不会**回退到仓库根 `brand/`。

本目录下的应用：

| 应用 | 路径 | 说明 |
|------|------|------|
| open | [`open/`](open/) | 标准窗 + WebView + 菜单/托盘 |
| shaped | [`shaped/`](shaped/) | 异形窗（无 WebView） |
| shaped_webview | [`shaped_webview/`](shaped_webview/) | 异形 + WebView |
| draw | [`draw/`](draw/) | 直绘画布 `#ui/draw`（无 WebView） |
| remote | [`remote/`](remote/) | 远程 HTTP 文档 + `xu://` IPC |
| remote_http | [`remote_http/`](remote_http/) | 仅测试用静态服（非桌面应用） |

## 打包 / 运行

`--desktop` **必须**指定应用目录（不能传 `.xu` 文件），目录内必须有 `xu.xun`。**macOS 直接产出 `.app`**（`-o Foo` → `Foo.app`；缺省用目录名）：

```bash
xu doctor --desktop --target macos-arm64

xu build examples/16_ui_webview/open \
 --target macos-arm64 --desktop -o /tmp/XuOpen
open /tmp/XuOpen.app

xu build examples/16_ui_webview/shaped \
 --target macos-arm64 --desktop -o /tmp/XuShaped
open /tmp/XuShaped.app

xu build examples/16_ui_webview/shaped_webview \
 --target macos-arm64 --desktop -o /tmp/XuShapedWebView

xu build examples/16_ui_webview/draw \
 --target macos-arm64 --desktop -o /tmp/XuDraw
```

其它 target：

```bash
xu build …/open --target macos-x64 --desktop -o /tmp/XuOpen
# windows/linux --desktop：本刀报「未实现」（不回退 ）
```

产品路径 **零 **：`xu doctor` / `xu build … --desktop` 在 `src/cmd/compile` 内编排（`--emit-obj` → XUOBJ→Mach-O → swiftc/clang/ar → `.app` + icns/codesign）。`scripts/pack_*.py` 仅 CI/调试。

`www/` 由 pack 按入口旁路嵌入；`brand/` 图标必须在应用目录内（缺则报错退出）。App 图标同时进 `Contents/Resources/AppIcon.icns`。

### 远程 HTTP 自测

```bash
python3 examples/16_ui_webview/remote_http/serve.py
xu build examples/16_ui_webview/remote \
 --target macos-arm64 --desktop -o /tmp/XuRemote
open /tmp/XuRemote.app
# 可选：XU_UI_REMOTE_URL=http://127.0.0.1:8765/
```

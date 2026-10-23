# 16 — 原生 UI 与 WebView 示例

这组示例覆盖 Xu 0.10.1 正式版的的绘图、文字输入、布局控件、样式、多窗口和异步桌面服务。先学习绘图与基本控件，再看 editor_workbench、style_gallery、webview_requests，最后用图片工作台串起各层。

| 示例 | 内容 | 主要操作 |
|---|---|---|
| [draw](draw/) | Frame、逻辑坐标、变换栈、裁剪、指针 | 点击圆、调整窗口 |
| [draw_gallery](draw_gallery/) | 路径、渐变、PNG、排版、图层、DisplayList、SurfaceCache | Space 切换缓存、R 失效 |
| [widgets](widgets/) | 约束布局、控件、焦点、捕获、滚动、重绘边界 | Tab、Enter、勾选、滚动 |
| [text_input](text_input/) | IME、预编辑、UTF-8 选区、光标几何 | 输入中文/emoji、选择、删除 |
| [desktop_services](desktop_services/) | 剪贴板、文件/保存/目录、提示、取消/超时 | 按 1–6、观察请求终态 |
| [multiwindow](multiwindow/) | 动态原生窗口、独立 Canvas、集中事件路由 | N/W/F/R、关闭首窗 |
| [workbench](workbench/) | 图片浏览与独立预览的综合应用 | 选择图片、缩放窗口 |
| [editor_workbench](editor_workbench/) | Input、TextArea、结构化菜单、屏幕查询 | 文字编辑、Tab、快捷键、Document 菜单 |
| [controls_gallery](controls_gallery/) | 扩展控件、焦点与 StyleSwitcher | 点击、拖动，按 T/S/D 切换样式 |
| [style_gallery](style_gallery/) | 统一主题、字体缩放、密度、局部覆盖、状态与滚动条 | 点击 Theme/Text/Density、编辑、拖动滑块 |
| [webview_requests](webview_requests/) | WebView 并发、取消、超时、大文本、原生查询 | Run all checks、请求中关闭窗口 |
| [open](open/) | 原有 WebView、菜单、托盘示例 | 网页与原生壳交互 |
| [shaped](shaped/) | 原有透明异形窗口 | 拖动窗口 |
| [remote](remote/) | 原有远程 WebView + Xu IPC | 先启动 remote_http 服务 |

## 统一入口

整组示例使用 **Xu 0.10.1 正式 SDK** 验收。默认从同级 `../xu` 找源码和 `bin/xu`；也可用 `XU_ROOT`、`XU` 或 `--compiler` 选择已解压的 SDK。

```sh
python3 scripts/ui_examples.py list
python3 scripts/ui_examples.py check all
python3 scripts/ui_examples.py build draw_gallery
python3 scripts/ui_examples.py run workbench
# 已打包后再次运行：
python3 scripts/ui_examples.py run workbench --no-build
# 指定目标（须具备对应工具链）：
python3 scripts/ui_examples.py build widgets --target windows-x64
# 同样可用 Make：
make ui-list
make ui-check UI=all
make ui-run UI=workbench
```

所有产物写入忽略的 `build/ui/<target>/`。macOS 输出 `.app`，Windows 输出 `.exe`，Linux 输出原生可执行文件。跨平台 `build` 由对应工具链执行，`run` 只允许本机目标；示例启动器不会自动安装系统依赖。WebView 示例还需要对应的 WebView 工具链。

使用产品命令直接打包时，当前桌面打包器从工作目录向上查找 `src/ui/host`，因此先进入 Xu 源码目录，再传入应用的绝对路径：

```sh
cd /path/to/xu
bin/xu doctor --desktop --target macos-arm64
bin/xu build /path/to/examples/examples/16_ui_webview/workbench \
  --target macos-arm64 --desktop -o /tmp/XuWorkbench
open /tmp/XuWorkbench.app
```

统一启动器会代为设置该工作目录。`--desktop` 接收含 `xu.xun` 的应用目录。每个应用自带 `brand/` PNG 图标；资源图库另有内嵌 `www/sample.png`，其余原生示例不需要 HTML。共享模块通过本地 `file:` 依赖导入，不需联网。

## 读代码时关注

- 只有一个 worker 消费 `App.event_ch`，先等 `canvas_ready` 再创建原生资源。
- `window_id`、`canvas_id`、generation 一起确定目标，关闭后不复用旧 Context。
- 一个完整 Frame 只提交一次；空闲不重绘；View 按脏节点更新并复用重绘边界。
- TextLayout、Image、DisplayList、SurfaceCache 和文本会话都有明确释放位置。尚未提交的 Frame 仅记录资源引用，相关句柄必须保留到 `present`、`freeze` 或 `snapshot` 成功；接受后才由原生帧持有资源版本。普通标签直接记录内联文字。
- 文件选择返回路径；剪贴板、提示和对话框通过请求终态完成，不阻塞事件消费者。
- `text_input` 演示低层 IME 会话；`editor_workbench` 展示可复用 Input / TextArea。滚动示例展示保留节点的可见区绘制，不宣称按需构建的虚拟列表。

各 README 给出操作、API 和生命周期说明。此处示例支持的具体能力以当前宿主实现与系统工具链为准。

## 远程网页

```sh
python3 examples/16_ui_webview/remote_http/serve.py
# 另一个终端：
python3 scripts/ui_examples.py run remote
# 可选 XU_UI_REMOTE_URL=http://127.0.0.1:8765/
```

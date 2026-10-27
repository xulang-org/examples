# shaped_webview

演示无标题栏、PNG 遮罩窗口与 WebView IPC；源码使用 macOS WebView 路径。

## 编译

本示例未注册到 `scripts/ui_examples.py`。在 macOS 上，从 examples 仓库根执行，使用配套的 Xu 源码及桌面工具链：

```sh
EXAMPLES_ROOT="$PWD"
XU_ROOT="${XU_ROOT:-$(dirname "$EXAMPLES_ROOT")/xu}"
mkdir -p "$EXAMPLES_ROOT/build/ui"
(
  cd "$XU_ROOT"
  XU_UI_PACK=1 bin/xu build "$EXAMPLES_ROOT/examples/16_ui_webview/shaped_webview" \
    --desktop --www "$EXAMPLES_ROOT/examples/16_ui_webview/shaped_webview/www" \
    -o "$EXAMPLES_ROOT/build/ui/shaped_webview"
)
```

`XU_ROOT` 需指向含 `src/ui/host` 和 `bin/xu` 的 Xu 源码目录；打包器从该目录定位宿主代码。macOS 产物为 `build/ui/shaped_webview.app`。

# zicq_ui

演示原生绘图窗口中的点击事件：读取 `/tmp/zicq-ping`，并将内容绘制到画布。

## 编译

本示例未注册到 `scripts/ui_examples.py`。在 examples 仓库根执行，需准备配套的 Xu 源码及桌面工具链：

```sh
EXAMPLES_ROOT="$PWD"
XU_ROOT="${XU_ROOT:-$(dirname "$EXAMPLES_ROOT")/xu}"
mkdir -p "$EXAMPLES_ROOT/build/ui"
(
  cd "$XU_ROOT"
  XU_UI_PACK=1 bin/xu build "$EXAMPLES_ROOT/examples/zicq_ui" \
    --desktop --no-webview -o "$EXAMPLES_ROOT/build/ui/zicq_ui"
)
```

`XU_ROOT` 需指向含 `src/ui/host` 和 `bin/xu` 的 Xu 源码目录；打包器从该目录定位宿主代码。macOS 产物为 `build/ui/zicq_ui.app`，Windows 为 `build/ui/zicq_ui.exe`，Linux 为 `build/ui/zicq_ui`。

本例使用原有的 `ui/draw` API，需匹配该 API 的编译器；桌面依赖与平台要求见 [UI 示例说明](../16_ui_webview/README.md#统一入口)。

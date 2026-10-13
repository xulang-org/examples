# draw — `#ui/draw` 画布

无 WebView。`#ui/draw`（`src/ui/draw`）是立即模式 2D 画布，对标 Love2D / Raylib / Cairo：图元、画笔、变换栈。不是流程图库。点击走 `draw.poll_click(app.event_ch)`（`#ui` 的 `ev_click`），不要自己读壳事件文件。

```bash
xu build examples/16_ui_webview/draw \
 --target macos-arm64 --desktop -o /tmp/XuDraw
open /tmp/XuDraw.app
```

示例里点蓝色圆会改底部文字。三端 pack 始终链入 `macos_draw.swift` / `linux_draw.cc` / `windows_draw.cc`。

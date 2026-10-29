# 动态多窗口

需要与本目录 `xu.xun` 的 `xu:` 要求匹配的 SDK，使用 `#ui` / `#ui/draw` 原生宿主，无 WebView。

演示 `open_content_window()`、独立 Canvas 身份、事件路由、窗口聚焦/调整大小/关闭和最后窗口退出。

## 运行

从本仓库根执行：

```sh
python3 scripts/ui_examples.py run multiwindow
# 与 xu 不在同级时：
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run multiwindow
# 仅检查或打包：
python3 scripts/ui_examples.py check multiwindow
python3 scripts/ui_examples.py build multiwindow
```

产物位于 `build/ui/<target>/`；macOS 为 `.app`，Windows 为 `.exe`，Linux 为可执行文件。打包需要对应平台工具链，使用 `xu doctor --desktop --target <target>` 检查。

## 操作

N 打开新窗口（示例最多同时 6 个）；各窗口点击计数独立；W 关闭当前窗口；F 聚焦首个存活窗口；R 调整当前窗口大小；Esc 退出全部窗口。尝试先关闭最初的窗口，其他窗口继续工作。

## 代码要点

只保留一个事件消费者，通过 `window_id` 路由，并检查 canvas generation。新窗口等到 `canvas_ready` 才提交首帧；每个窗口使用自己的绘图 Context。示例窗口数量上限与标准库原生上限独立。

入口见 [main.xu](main.xu)。共享的窗口启动、事件等待和文字辅助函数见 [shared/](../shared/)；应用仍自行持有模型、资源和事件分发，不另开事件消费者。

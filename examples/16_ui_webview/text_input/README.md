# 原生 IME 文本会话

需要与本目录 `xu.xun` 的 `xu:` 要求匹配的 SDK，使用 `#ui` / `#ui/draw` 原生宿主，无 WebView。

演示 `TextInput`、原生输入法预编辑、UTF-8 字节选区、排版绑定、命中测试和光标几何。

## 运行

从本仓库根执行：

```sh
python3 scripts/ui_examples.py run text_input
# 与 xu 不在同级时：
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run text_input
# 仅检查或打包：
python3 scripts/ui_examples.py check text_input
python3 scripts/ui_examples.py build text_input
```

产物位于 `build/ui/<target>/`；macOS 为 `.app`，Windows 为 `.exe`，Linux 为可执行文件。打包需要对应平台工具链，使用 `xu doctor --desktop --target <target>` 检查。

## 操作

切换系统输入法输入中文；输入 emoji；方向键、Shift+方向键和 Backspace 操作文本；点击输入区移动光标；Esc 退出。

## 代码要点

原生会话持有权威修订号。显示使用 `display_value()`，包含尚未提交的预编辑；事件只触发重新读取快照，不重复应用原生编辑。`bind_input()` 绑定实际绘制的布局；预编辑期间点击不会覆盖组合文本。这是单行低层会话示例，尚未封装为 Input/TextArea 控件。

入口见 [main.xu](main.xu)。共享的窗口启动、事件等待和文字辅助函数见 [shared/](../shared/)；应用仍自行持有模型、资源和事件分发，不另开事件消费者。

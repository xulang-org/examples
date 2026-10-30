# 扩展控件与 StyleSwitcher

使用满足本目录 `xu.xun` 中 `xu:` 要求的 SDK，从 examples 仓库根执行：

```sh
python3 scripts/ui_examples.py check controls_gallery
python3 scripts/ui_examples.py build controls_gallery
python3 scripts/ui_examples.py run controls_gallery
```

默认使用同级 `../xu`，可用 `XU_ROOT` 指定匹配的 Xu 源码目录。

展示 Switch、Radio、Slider、Stepper、Tabs、Select、ListBox、Badge、Divider 和 Card。点击或拖动控件；Tab 切换焦点，再用方向键、Space 或 Enter 操作。单个 Radio 演示选择状态，多选项的互斥关系由应用管理。Select 的基础样式绘制器重复同一个标签，当前选项序号显示在底部。

按 T 用 `StyleSwitcher.next()` 切换深色、浅色、高对比主题；S 切换文字缩放；D 切换紧凑密度。每次从 `styles()` 取得完整样式，用各控件的 `paint_styled()` 绘制。固定矩形便于对照每种控件；保留布局见 style_gallery；使用 `StyleSwitcher.apply(view)` / `cycle(view)` 可将同一套样式应用到 View，详见官网 API。

窗口中的事件只由一个 worker 消费。匹配 window_id 与 generation，等待 canvas_ready 后创建资源；标签保留到整帧提交成功，关闭后不再使用失效的原生句柄。空闲时等待事件，不持续重绘。

事件通过 `ActionDispatcher` 按控件 ID 注册（`on` / `on_any`），在循环中 `dispatch`。
请使用包含该 API 的当前 Xu 源码/SDK；样式配置在独立样式路径处理。

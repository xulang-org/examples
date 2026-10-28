# 保留式布局与控件

需要与本目录 `xu.xun` 的 `xu:` 要求匹配的 SDK，使用 `#ui` / `#ui/draw` 原生宿主，无 WebView。

演示 `View`、列布局、约束、Button、Checkbox、Progress、Scroll、重绘边界、指针捕获和键盘焦点。

## 运行

从本仓库根执行：

```sh
python3 scripts/ui_examples.py run widgets
# 与 xu 不在同级时：
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run widgets
# 仅检查或打包：
python3 scripts/ui_examples.py check widgets
python3 scripts/ui_examples.py build widgets
```

产物位于 `build/ui/<target>/`；macOS 为 `.app`，Windows 为 `.exe`，Linux 为可执行文件。打包需要对应平台工具链，使用 `xu doctor --desktop --target <target>` 检查。

## 操作

点击按钮增加计数和进度；取消勾选后按钮不可用；Tab/Shift+Tab 切换焦点，Enter/Space 激活控件；在列表滚动，聚焦列表后使用 Home/End/PageUp/PageDown；调整窗口验证布局。

## 代码要点

布局和绘制复用同一棵树。`dispatch_event()` 统一处理输入、焦点和关闭；业务状态改变后仅使受影响节点失效。控制台打印重绘边界复用数量。20 行均保留在树中，只裁掉不可见绘制，并非按需创建的虚拟列表。

入口见 [main.xu](main.xu)。共享的窗口启动、事件等待和文字辅助函数见 [shared/](../shared/)；应用仍自行持有模型、资源和事件分发，不另开事件消费者。

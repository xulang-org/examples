# 图片工作台

需要与本目录 `xu.xun` 的 `xu:` 要求匹配的 SDK，使用 `#ui` / `#ui/draw` 原生宿主，无 WebView。

把保留式布局、键盘可操作控件、异步文件选择、PNG/JPEG 解码、按比例显示、显示列表缓存和独立预览窗口串成一个应用。

## 运行

从本仓库根执行：

```sh
python3 scripts/ui_examples.py run workbench
# 与 xu 不在同级时：
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run workbench
# 仅检查或打包：
python3 scripts/ui_examples.py check workbench
python3 scripts/ui_examples.py build workbench
```

产物位于 `build/ui/<target>/`；macOS 为 `.app`，Windows 为 `.exe`，Linux 为可执行文件。打包需要对应平台工具链，使用 `xu doctor --desktop --target <target>` 检查。

## 操作

点击 Open a PNG or JPEG 选择图片；勾选预览选项可打开第二个原生窗口；再次选择图片会更新主窗口和预览；调整窗口大小验证约束与图片比例；Esc 退出。

## 代码要点

主窗口与预览窗口分别创建图片资源，避免跨 Canvas 使用句柄。替换图片时释放旧资源并使图片节点失效；取消或解码失败显示状态，不丢掉当前图片。关闭主窗口结束工作台，关闭预览不结束主窗口。

入口见 [main.xu](main.xu)。共享的窗口启动、事件等待和文字辅助函数见 [shared/](../shared/)；应用仍自行持有模型、资源和事件分发，不另开事件消费者。

事件通过 `ActionDispatcher` 按控件 ID 注册（`on` / `on_any`），在循环中 `dispatch`。
请使用包含该 API 的当前 Xu 源码/SDK；样式配置在独立样式路径处理。

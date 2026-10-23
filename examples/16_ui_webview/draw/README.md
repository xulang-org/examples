# 基础帧绘图

需要 **Xu 0.10.0 或更新版本**，使用 `#ui` / `#ui/draw` 原生宿主，无 WebView。

从旧的逐条命令改为 `Context → Frame → present()`，演示逻辑坐标、状态栈、变换、裁剪和指针事件。

## 运行

从本仓库根执行：

```sh
python3 scripts/ui_examples.py run draw
# 与 xu 不在同级时：
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run draw
# 仅检查或打包：
python3 scripts/ui_examples.py check draw
python3 scripts/ui_examples.py build draw
```

产物位于 `build/ui/<target>/`；macOS 为 `.app`，Windows 为 `.exe`，Linux 为可执行文件。打包需要对应平台工具链，使用 `xu doctor --desktop --target <target>` 检查。

## 操作

点击青色圆增加计数；调整窗口大小触发重绘；Esc 退出。

画布默认保持固定，拖拽和滚轮不会自动移动整幅图。Xu 0.10.1-dev.1 起三个桌面后端都默认关闭相机平移；需要移动画布的应用可在 `canvas_ready` 后调用 `draw.set_pan_enabled(1)`。示例也显式关闭平移，以兼容 Xu 0.10.0。

## 代码要点

`Frame` 在事件循环外创建并复用容量，每次完整记录后只提交一次；空闲时不持续绘制。

入口见 [main.xu](main.xu)。共享的窗口启动、事件等待和文字辅助函数见 [shared/](../shared/)；应用仍自行持有模型、资源和事件分发，不另开事件消费者。

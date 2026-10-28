# UI 示例共享辅助模块

`github.com/xulang-org/examples/ui_demo` 通过各示例的 `file:../shared` 显式依赖，仅提供窗口启动、单消费者事件等待、配色及内联文字命令辅助函数。示例不需要联网下载模块。

## 依赖构建

本目录是库模块，没有独立可执行入口。在仓库根构建依赖它的应用时会一并编译，例如：

```sh
python3 scripts/ui_examples.py build draw
```

Xu 源码、桌面工具链与产物位置见[父目录说明](../README.md#统一入口)。

应用自身仍负责持有资源、处理 canvas generation、分发事件与清理生命周期。`next()` 在队列为空时短暂休眠；各示例按事件重绘，不创建无条件刷新循环。

显式 `TextLayout` 资源应由调用者保活至 `present/freeze/snapshot` 成功，再释放用户句柄；尚未接受的帧只有资源引用。

`windows()` 创建只用于动态窗口命令的 App 值，复用主 App 的事件通道，不启动第二个事件泵；应用只对主 App 调用 `run()`。`ready()` 收到 `canvas_ready` 后，通过窗口身份启用逻辑输入并关闭相机平移，避免命令先于原生画布安装。

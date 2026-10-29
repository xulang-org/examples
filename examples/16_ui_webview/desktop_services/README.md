# 异步桌面服务

需要与本目录 `xu.xun` 的 `xu:` 要求匹配的 SDK，使用 `#ui` / `#ui/draw` 原生宿主，无 WebView。

演示剪贴板读写、文件多选、保存路径、文件夹多选、输入提示、请求匹配、取消、超时和不同终态。

## 运行

从本仓库根执行：

```sh
python3 scripts/ui_examples.py run desktop_services
# 与 xu 不在同级时：
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run desktop_services
# 仅检查或打包：
python3 scripts/ui_examples.py check desktop_services
python3 scripts/ui_examples.py build desktop_services
```

产物位于 `build/ui/<target>/`；macOS 为 `.app`，Windows 为 `.exe`，Linux 为可执行文件。打包需要对应平台工具链，使用 `xu doctor --desktop --target <target>` 检查。

## 操作

点击对应按钮或按 1–6；同一时间保留一个请求；可点击取消按钮请求取消；提示框 15 秒超时；Esc 退出。

## 代码要点

`submit` 成功仅代表受理，结果在同一个 `App.event_ch` 的 `ev_request` 中到达。用 `matches()` 校验请求身份，并区别 completed/cancelled/timed_out/failed/unsupported。选择保存路径不写入文件；读取的剪贴板内容只显示在本地窗口。系统模态框打开时，窗口内取消按钮可能不可点击，可使用系统取消按钮或等待超时。

入口见 [main.xu](main.xu)。共享的窗口启动、事件等待和文字辅助函数见 [shared/](../shared/)；应用仍自行持有模型、资源和事件分发，不另开事件消费者。

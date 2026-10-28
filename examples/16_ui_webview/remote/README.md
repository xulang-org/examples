# remote

演示远程 HTTP 页面与原生 WebView 之间的 Xu IPC。

## 编译与运行

在仓库根目录执行，需准备匹配的 Xu 源码及桌面工具链，详见[父目录说明](../README.md#统一入口)：

```sh
python3 scripts/ui_examples.py build remote
# 可选：启动已打包的应用
python3 scripts/ui_examples.py run remote --no-build
```

启动器默认使用同级 `../xu`，可用 `XU_ROOT` 选择其他 Xu 源码目录；产物写入 `build/ui/<target>/`。

运行前在另一个终端从仓库根启动配套网页服务：

```sh
python3 examples/16_ui_webview/remote_http/serve.py
```

默认网页地址为 `http://127.0.0.1:8765/`，可通过 `XU_UI_REMOTE_URL` 更改。

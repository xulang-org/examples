# open

演示本地 WebView 页面与 Xu 之间的 IPC，以及原生菜单和托盘操作。

## 编译与运行

在仓库根目录执行，需准备匹配的 Xu 源码及桌面工具链，详见[父目录说明](../README.md#统一入口)：

```sh
python3 scripts/ui_examples.py build open
# 可选：启动已打包的应用
python3 scripts/ui_examples.py run open --no-build
```

启动器默认使用同级 `../xu`，可用 `XU_ROOT` 选择其他 Xu 源码目录；产物写入 `build/ui/<target>/`。

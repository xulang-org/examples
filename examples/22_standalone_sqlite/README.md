# 22_standalone_sqlite

演示独立 SQLite 驱动的本地数据库、参数化查询与事务操作。

## 准备原生桥

以下命令从 `examples` 仓库根执行，默认同级检出 `../xu` 与 `../packages`，并将 Xu 编译器 `xu` 加入 `PATH`。
`xu.xun` 中的 `file:../../../packages` 引用同级 packages 根模块；改变目录布局时须同步调整。

清洁检出不含 `../packages/sqlite/native/sqlite.xulib`，必须先生成它：

```sh
make -C ../packages native
```

这一步需要 Python 3、Clang、`llvm-ar`（或 `ar`）及 macOS SDK（Xcode 或 Command Line Tools）。
默认调用同级 `../xu/scripts/xunative.py`；若 Xu 源码在别处，可执行 `XU_ROOT=/absolute/path/to/xu make -C ../packages native`。
桥使用 packages 中已附带的 SQLite 源码，无需安装系统 SQLite 库。

`make native` 默认只打包 `macos-arm64` 与 `macos-x64` 两个切片；下面的完整编译和运行流程面向 macOS。
当前 Xu 的一步原生静态链接也仅接受这两个目标。Linux/Windows 上即使另行生成对应切片，也不能直接照抄下面的可执行文件构建命令；`--emit-obj` 仅输出 XUOBJ，仍需另行完成目标对象转换与系统链接。

## 编译与运行

完成上面的准备后，仍在 `examples` 仓库根执行：

```sh
mkdir -p build
xu build examples/22_standalone_sqlite -o build/22_standalone_sqlite
./build/22_standalone_sqlite
```

本目录以 `main.xu` 为入口；按目录构建会使用 `xu.xun` 的依赖声明。
不需要外部数据库服务；程序会创建并删除本地 `/tmp/xu_sqlite_test.db`，请勿在该路径放置需要保留的数据。

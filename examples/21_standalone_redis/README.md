# 21_standalone_redis

演示独立 Redis 驱动的命令编码、键值操作、流水线与连接池。

## 编译

在仓库根目录执行，需已将 Xu 编译器 `xu` 加入 `PATH`：

```sh
mkdir -p build
xu build examples/21_standalone_redis -o build/21_standalone_redis
```

本目录以 `main.xu` 为入口；按目录构建会使用 `xu.xun` 的依赖声明。
清单中的 `file:../../../packages` 指向与本仓库同级的 `../packages`，编译前需准备该模块。
编译不连接外部服务；运行所需的服务地址和配置见 `main.xu`。

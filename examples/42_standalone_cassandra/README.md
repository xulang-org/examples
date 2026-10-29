# 42_standalone_cassandra

演示独立 Cassandra 客户端的 CQL 数据操作与连接池。

## 编译

在仓库根目录执行，需已将 Xu 编译器 `xu` 加入 `PATH`：

```sh
mkdir -p build
xu build examples/42_standalone_cassandra -o build/42_standalone_cassandra
```

本目录以 `main.xu` 为入口；按目录构建会使用 `xu.xun` 的依赖声明。
清单中的 `file:../../../packages` 指向与本仓库同级的 `../packages`，编译前需准备该模块。
编译不连接外部服务；运行所需的服务地址和配置见 `main.xu`。

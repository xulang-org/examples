# 示例索引

<p align="center"><img src="../brand/xu_classic.svg" alt="Xu" width="72" height="72"></p>

01–10 对应 [`USAGE.md`](../USAGE.md) 的语言教程与 [`FEATURES.md`](../FEATURES.md) 的特色表；其余目录覆盖网络、数据库、UI、原生互操作与 WASM。各目录 README 提供具体构建步骤和运行条件。

## 构建入口

以下命令从本仓库根执行，默认与 `../xu`、`../packages` 同级检出。先构建同级 Xu 的产品编译器，或用 `XU` 指定与 `XU_ROOT` 源码配套的可执行文件：

```sh
make check EXAMPLE=01_hello
make build EXAMPLE=01_hello
make run EXAMPLE=01_hello
# 非默认编译器与源码目录：
XU=/absolute/path/to/xu XU_ROOT=/absolute/path/to/xu-source make run EXAMPLE=01_hello
```

通用入口支持下表中的普通 Xu 程序；UI、原生库和 WASM 的特殊流程见各组说明。网络与数据库示例运行前须按各自 README 和 `main.xu` 准备服务、连接配置与测试数据。编译通常不连接这些服务。

驱动示例通过 `xu.xun` 中的 `file:../../../packages` 引用同级 packages 模块，改变目录布局时须同步修改依赖路径。SQLite 示例须先准备原生桥，当前一步链接流程仅支持 macOS，详见 [22_standalone_sqlite](22_standalone_sqlite/README.md) 和 [36_test_sqlite_pool](36_test_sqlite_pool/README.md)；[28_test_pools](28_test_pools/README.md) 同样导入 SQLite，也需要该准备步骤。

## 基础语言

| 目录 | 内容与运行提示 |
|---|---|
| [01_hello](01_hello/README.md) | 程序骨架与输出（§1） |
| [02_bindings](02_bindings/README.md) | 绑定与常量（§2） |
| [03_control_flow](03_control_flow/README.md) | 控制流（§4） |
| [04_functions_methods](04_functions_methods/README.md) | 函数、结构体与方法（§5） |
| [05_errors](05_errors/README.md) | 错误处理（§6） |
| [06_ownership_borrow](06_ownership_borrow/README.md) | 所有权与借用（§7） |
| [07_arrays_strings](07_arrays_strings/README.md) | 数组与字符串（§8） |
| [08_enum_match](08_enum_match/README.md) | Enum 与 match（§9） |
| [09_generics_temp](09_generics_temp/README.md) | 泛型、struct@ 与 any（§10） |
| [10_concurrency](10_concurrency/README.md) | 协程与信道（§11） |

## HTTP、TCP 与 UDP

| 目录 | 内容与运行提示 |
|---|---|
| [11_http_server](11_http_server/README.md) | HTTP 路由 |
| [12_file_server](12_file_server/README.md) | HTTP 文件服务 |
| [13_http_context](13_http_context/README.md) | HTTP Context 与 mux |
| [14_tcp_echo](14_tcp_echo/README.md) | TCP echo |
| [15_udp_echo](15_udp_echo/README.md) | UDP echo |

## UI 与设计系统

| 目录 | 内容与运行提示 |
|---|---|
| [16_ui_webview](16_ui_webview/README.md) | 原生 UI、绘图、控件与 WebView。使用 `python3 scripts/ui_examples.py list` 查看统一入口；设计系统 HTTP 服务和未注册应用按各自 README 构建。 |

## 原生互操作

| 目录 | 内容与运行提示 |
|---|---|
| [19_native_c](19_native_c/README.md) | C → XUNATIVE 归档 → Xu 静态链接。先生成 `native/native_add.xulib`，再使用通用构建入口。 |
| [20_c_export_lib](20_c_export_lib/README.md) | Xu 导出静态库、动态库与多语言调用。使用目录内的 Makefile 或 `build_and_test.sh`，工具链要求见 README。 |

## 数据库与独立驱动

| 目录 | 内容与运行提示 |
|---|---|
| [17_database_postgres](17_database_postgres/README.md) | database/sql 与 PostgreSQL |
| [18_database_mysql](18_database_mysql/README.md) | database/sql 与 MySQL/MariaDB |
| [21_standalone_redis](21_standalone_redis/README.md) | 独立 Redis 驱动的命令编码、键值操作、流水线与连接池 |
| [22_standalone_sqlite](22_standalone_sqlite/README.md) | 独立 SQLite 驱动的本地数据库、参数化查询与事务操作 |
| [23_standalone_memcached](23_standalone_memcached/README.md) | 独立 Memcached 客户端的缓存读写与计数操作 |
| [24_standalone_nats](24_standalone_nats/README.md) | 独立 NATS 客户端的发布订阅与消息收发 |
| [25_standalone_mqtt](25_standalone_mqtt/README.md) | 独立 MQTT 客户端的主题订阅与消息发布 |
| [26_standalone_elasticsearch](26_standalone_elasticsearch/README.md) | 独立 Elasticsearch 客户端的索引、文档操作与连接池 |
| [27_standalone_s3](27_standalone_s3/README.md) | 独立 S3 客户端的签名计算、对象存储访问与连接池 |
| [28_test_pools](28_test_pools/README.md) | 集中演示 Redis、Memcached、NATS、MQTT、Elasticsearch、S3、SQLite 和 PostgreSQL 的连接池 |
| [29_standalone_clickhouse](29_standalone_clickhouse/README.md) | 独立 ClickHouse 客户端的查询与连接池 |
| [30_standalone_etcd](30_standalone_etcd/README.md) | 独立 etcd 客户端的键值操作、租约与连接池 |
| [31_standalone_mongodb](31_standalone_mongodb/README.md) | 独立 MongoDB 客户端的握手、文档操作与连接池 |
| [32_standalone_influxdb](32_standalone_influxdb/README.md) | 独立 InfluxDB 客户端的健康检查、数据写入、查询与连接池 |
| [33_standalone_kafka](33_standalone_kafka/README.md) | 独立 Kafka 客户端的连接检查、消息生产、拉取与连接池 |
| [34_standalone_postgres](34_standalone_postgres/README.md) | 独立 PostgreSQL 驱动的数据库访问与连接池 |
| [35_standalone_mysql](35_standalone_mysql/README.md) | 独立 MySQL 驱动的数据库访问与连接池 |
| [36_test_sqlite_pool](36_test_sqlite_pool/README.md) | SQLite 连接池的复用与数据库操作 |
| [37_standalone_rabbitmq](37_standalone_rabbitmq/README.md) | 独立 RabbitMQ 客户端的消息操作与连接池 |
| [38_standalone_qdrant](38_standalone_qdrant/README.md) | 独立 Qdrant 客户端的向量数据库访问与连接池 |
| [39_standalone_consul](39_standalone_consul/README.md) | 独立 Consul 客户端的服务访问与连接池 |
| [40_standalone_prometheus](40_standalone_prometheus/README.md) | Prometheus 指标文本生成与 Pushgateway 连接池 |
| [41_standalone_neo4j](41_standalone_neo4j/README.md) | 独立 Neo4j 客户端的 Cypher 数据操作与连接池 |
| [42_standalone_cassandra](42_standalone_cassandra/README.md) | 独立 Cassandra 客户端的 CQL 数据操作与连接池 |
| [43_standalone_nacos](43_standalone_nacos/README.md) | 独立 Nacos 客户端的服务注册、配置访问与连接池 |
| [44_standalone_chroma](44_standalone_chroma/README.md) | 独立 Chroma 客户端的集合、数据操作与连接池 |
| [45_standalone_zookeeper](45_standalone_zookeeper/README.md) | 独立 ZooKeeper 客户端的四字命令与连接池 |
| [46_standalone_grpc](46_standalone_grpc/README.md) | Protobuf 字段编码、gRPC 一元调用与连接池 |
| [47_standalone_websocket](47_standalone_websocket/README.md) | WebSocket 握手、帧掩码、回显与协程连接池 |
| [48_standalone_smtp](48_standalone_smtp/README.md) | 独立 SMTP 客户端的邮件事务与连接池 |
| [49_standalone_dns](49_standalone_dns/README.md) | DNS 报文编码、应答解析与 TCP DNS 连接池 |
| [50_standalone_milvus](50_standalone_milvus/README.md) | 独立 Milvus 客户端的集合配置、向量实体与连接池 |
| [51_standalone_meilisearch](51_standalone_meilisearch/README.md) | 独立 Meilisearch 客户端的健康检查、索引、文档写入与连接池 |

## WebAssembly

| 目录 | 内容与运行提示 |
|---|---|
| [52_wasm](52_wasm/README.md) | Xu → WASM 的整数计算与 frame ABI。使用 `make wasm`（需要 Node.js）；浏览器预览步骤见 README。 |

设计系统的独立 HTTP 入口见 [xu_design_system](16_ui_webview/xu_design_system/README.md)；另有 [zicq_ui](zicq_ui/README.md) 聊天界面示例。

交叉编译普通语言示例时，可在仓库根执行（`xu` 已加入 `PATH`）：

```sh
mkdir -p build
xu build examples/01_hello/main.xu --target linux-x64 -o build/hello_linux_x64
```

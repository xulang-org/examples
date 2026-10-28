# 18_database_mysql

用 `database/sql` 调用 MySQL 或 MariaDB。驱动由独立模块 `packages/mysql` 提供，通过 `mysql #github.com/xulang-org/packages/mysql` 导入；标准库只提供通用 SQL API 和驱动接口。

本目录的 `xu.xun` 已声明本地依赖：

```xun
require:
  "github.com/xulang-org/packages": file:../../../packages
```

示例覆盖：

- `sql.open(mysql.driver(), dsn)` 与 `ping`
- 建表及参数化 `exec`（占位符为 `?`）
- `prepare`、事务 `begin` / `commit`
- `query` + `rows.next()` / `rows.take()`
- `query_row` 与关闭连接

## 启动本地 MariaDB

当前驱动支持 `mysql_native_password`，所以这里使用默认兼容该认证方式的 MariaDB：

```bash
docker run --rm --name xu-mariadb \
 -e MARIADB_ROOT_PASSWORD=root \
 -e MARIADB_DATABASE=xu_demo \
 -p 3306:3306 \
 mariadb:11
```

等容器就绪后，在另一个终端编译并运行：

```bash
XU="$(pwd)/../xu/bin/xu"
export XU_MYSQL_DSN='root:root@tcp(127.0.0.1:3306)/xu_demo?tls=false'
../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$XU" build examples/18_database_mysql/main.xu -o /tmp/xu_18_mysql
chmod +x /tmp/xu_18_mysql && /tmp/xu_18_mysql
```

未设置 `XU_MYSQL_DSN` 时，程序使用上面的本地 DSN。也支持 URL：

```text
mysql://root:root@127.0.0.1:3306/xu_demo
```

当前驱动使用 MySQL 文本协议，参数由驱动按 SQL 上下文转义；Prepare 目前是 `database/sql` 形状，不是服务端 `COM_STMT_EXECUTE`。当前 `database/sql` 单次调用最多传 3 个参数。本示例显式使用 `tls=false`，仅适合本地演示。驱动已支持 `tls=true`、`required` 和 `verify`，通过 CLIENT_SSL 在同一连接上升级到 TLS，并校验证书链与主机名；自签名根证书用 `tlsrootcert=/absolute/path/ca.pem` 指定。升级或校验失败时不会回退到明文。认证仍需服务端启用兼容的 `mysql_native_password`；本例使用 MariaDB，不保证适用于所有 MySQL 默认认证配置。

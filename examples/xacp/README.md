# XACP 最小样例

以下命令从 **xulang-org/examples 仓库根目录** 执行。需要已安装且包含
`xu-xacp` 工具的 Xu SDK，`xu` 应位于 `PATH`；也可使用同级 `../xu` checkout
的产品编译器与配套工具：

```sh
# 已安装 SDK：
XU=xu
# 或 sibling checkout（可用 XU_ROOT 指定其他 Xu 源码目录）：
# export XU_ROOT="${XU_ROOT:-../xu}"
# make -C "$XU_ROOT" xu-xacp
# XU="$XU_ROOT/bin/xu"

mkdir -p build
"$XU" build --xacp examples/xacp/main.xu -o build/xacp-example
./build/xacp-example --xacp-help
"$XU" xacp inspect ./build/xacp-example
"$XU" xacp debug ./build/xacp-example -- 'hello world'
"$XU" xacp run ./build/xacp-example -- --xacp-help
"$XU" xacp record ./build/xacp-example -- hello > build/xacp-output.txt
"$XU" build examples/xacp/core/main.xu -o build/xacp-core
./build/xacp-core # 输出 XACP core: PASS
```

完整工具回归属于 Xu 仓库。有 sibling checkout 时，可从本仓库根执行：

```sh
XU_EXAMPLES_ROOT="$PWD" make -C "${XU_ROOT:-../xu}" xacp-test
```

迁自 [xu 的原始提交](https://github.com/xulang-org/xu/commit/9dfc27327b397dec04570c27ee15d9c81220c9be)；
[stdio 设计记录](STDIO-CONTROL-DESIGN.md) 保留历史方案，当前用法以本 README 为准。

`--xacp` 是构建选项；单独导入 `#xacp` 不会生成启动发现入口。
`--xacp-port PORT`（或 `--xacp-port=PORT`）会同时启用 XACP，端口范围为
0..65535。端口 0 禁用 TCP；非零端口由显式 `--xacp-tcp` 入口在 `127.0.0.1` 监听。
另一个终端运行 `xu xacp connect PORT`，通过 stdin/stdout 逐帧请求/响应。
TCP 与 `--xacp-stdio` 均独占控制、不运行业务 main；`--` 前首个控制标志优先。
每条连接独立会话，连接顺序处理；结束服务可终止进程。普通启动不监听。
生成器目前只允许 hosted native 可执行程序，拒绝 lib/shared/test、嵌入式和
`--desktop` 打包模式。

发现信息保留 `stdio` 能力，并标明 TCP 是否启用及绑定地址：`inspect` 执行目标的 `--xacp-help` 并输出结果；
`debug` 先发现再启动，不提供断点调试；`run` 直接启动；`record` 在 Unix 上录制
最多 1 MiB stdout，stderr 仍继承终端，Windows 暂不支持。录制输出不是 XACP
UI 事件/帧文件。目标必须是受信任的本地可执行程序；inspect 同样会执行它。

用户参数必须写在工具的 `--` 后；工具会给目标额外插入 `--`，目标的
`os.args()` 会保留该标记。生成的启动扫描遇到标记即停止，所以业务参数
`--xacp-help` 不会触发发现。路径建议使用 `./build/...` 或绝对路径。
inspect 和 control 不接受额外业务参数，子进程失败会返回非零状态。

[Xu 标准库 `xacp/mod.xu`](https://github.com/xulang-org/xu/blob/main/src/xacp/mod.xu) 提供有界帧/XUN 校验、权限、会话生命周期等安全核心；
[`xacp/ui.xu`](https://github.com/xulang-org/xu/blob/main/src/xacp/ui.xu) 提供 UI 适配及录制 API。`--xacp` 构建自动接入 [`xacp/stdio.xu`](https://github.com/xulang-org/xu/blob/main/src/xacp/stdio.xu)。
`xu xacp control <program>` 继承调用方的 stdin/stdout 管道，启动目标的
`--xacp-stdio` 独占模式，不运行业务 main；普通运行不读取控制 stdin。
hello 校验并预约请求后、finish_request 前调用 enable_development。
proc.status 返回 pid/state；proc.output 与全部 UI ops 经过 schema/权限/生命周期
校验，无注册适配器时返回 not_implemented（包括 mutate、record）。
ScreenAdapter 的真实 UI 数据桥接仍须应用接入；本阶段没有事件推送。

例如：`"$XU" xacp control ./build/xacp-example < examples/xacp/control.xun`。
响应沿用帧外壳，包含 id、ok 和 data 或 error.code；无效帧/schema 的 id 为 0。
EOF 关闭会话，半帧 EOF 返回 truncated_frame，超限返回 frame_too_large 并关闭该会话。
TCP 监听继续接收后续连接；stdio 模式则退出。
持续无输出/不退出的目标尚无超时或终止机制，调用方应自行设置进程超时。

TCP 示例（先运行监听，再在另一终端连接）：

```sh
"$XU" build --xacp-port 4321 examples/xacp/main.xu -o build/xacp-example
./build/xacp-example --xacp-tcp
# 另一终端（同样从 examples 仓库根运行，并设置 XU）：
"$XU" xacp connect 4321 < examples/xacp/control.xun
```

客户端逐帧等待响应，输入帧必须用 `===` 加换行结束；输入 EOF 后断开连接。

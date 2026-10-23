# WebView request lifecycle

使用 Xu 0.10.1-dev.2 的本地 WebView 示例，无 HTTP 服务器。

```sh
python3 scripts/ui_examples.py run webview_requests
```

“Run all checks” 验证并发乱序返回、200 KB UTF-8 / 换行 / NUL、显式取消、AbortSignal、超时，以及按窗口身份发起的原生 JavaScript 查询。界面显示每项结果；“Start work, then close window” 可观察有请求时关闭窗口的清理。

`__XU__.invoke(name, payload, {timeoutMs, signal})` 返回 Promise，带 `.id` 与 `.cancel()`。Xu 用 `bind_ipc_requests_www` 接收 `InvokeRequest`，其中包含请求 ID、window_id、generation、name 和 payload。原生侧只跨任务传递 ID，工作任务从注册表领取请求体。

最多 64 个活跃派发额度，单个请求体（包含协议头）/ 返回值上限 1 MiB，排队请求体与结果共用 8 MiB。取消不能强制中断用户回调；迟到结果被丢弃，回调返回后释放额度。关闭、导航会撤销所属文档的待处理请求，回调在 UI 线程完成。

`WebView.eval_async`、`find_async`、`cookies_async`、`set_cookie_async` 和 `clear_cookies_async` 返回 `ui.Request`，经 `ui.ev_request` 完成，支持同一套取消、超时与窗口身份校验；原生结果上限 64 KiB。兼容的等待接口不消费 App 的公开事件队列。

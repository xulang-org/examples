# 内嵌 WebView 组件

两个系统 WebView 与原生 Canvas 控件共享一个窗口。左侧编辑预览、右侧参考页面拥有独立文档、请求身份、加载状态和生命周期。

```xu
id := screen.webview() or >>
screen.place(id,28.0,174.0,460.0,450.0) or >>
screen.load_web_html(id,html) or >>
// 或 screen.load_web_url(id,"https://example.com")
```

使用 `screen.webview_native(id)` 与 `webview.bind_native(&native)` 可调用完整的 WebView 异步查询接口。IPC 回调使用 `request.target()` 获取发起请求的确切组件；不要将 `request.window_id` 当作组件的 Canvas ID。

- `set_visible` 隐藏后保留页面状态，恢复时不重载。
- `set_enabled` 管理鼠标与键盘交互；`focus` 加入 Screen 焦点顺序。
- 网页内 Tab 按 DOM 顺序移动；Ctrl+Tab / Ctrl+Shift+Tab 返回原生控件。
- `close_webview` 立即撤销所有副本与待处理请求；父窗口关闭也会回收所有子视图。
- `ACTION_WEB_LOADING / LOADED / FAILED / PROGRESS` 的 `id` 是 Screen 组件句柄。
- `ui.native_view` 提供独立使用的原生子视图 API；`update` 接受矩形、裁剪范围、圆角、可见、启用与焦点状态。

需要在打包时链接 WebView 后端，不能使用 `--no-webview`。组件采用系统原生表面：与 Canvas 弹层重叠时暂时隐藏网页，弹层关闭后恢复；Canvas 的旋转、透明度与截图不会自动合成网页。`Screen.snapshot` 会明确拒绝包含可见 WebView 的导出。

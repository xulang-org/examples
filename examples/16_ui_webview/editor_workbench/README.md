# Writing desk

Xu 0.10.2 正式版 原生 Input、TextArea、结构化菜单与显示器查询示例。

```sh
python3 scripts/ui_examples.py run editor_workbench
```

- 点击输入，Tab / Shift+Tab 切换焦点，单行 Enter 提交，多行 Enter 换行。
- 支持拖动选区、方向键、Home / End、Shift 扩展选区，以及系统复制、剪切、粘贴快捷键。多行超出视口后可滚动。
- Document 菜单提供重置、编辑开关、子菜单与禁用项；主修饰键 + Shift + R 重置，主修饰键 + Shift + D 查询显示器。
- 页脚显示真实屏幕查询结果。Windows 屏幕矩形使用物理像素；macOS / GTK 使用逻辑坐标。GTK 无法可靠获取主屏与工作区时，结果的 known 标记为 false。

`Editor` 保留控件值，聚焦时使用原生 `TextInput` 会话处理文字和 IME。`View.layout` 后调用 `place_editor`，使命中、绘制与输入法候选位置一致。原生文字事件与剪贴板请求结果都交给 `Editor.event`；键盘和指针交给 `widgets.dispatch_event`。剪切、粘贴只对请求时的会话版本生效，焦点切换或更新后会丢弃迟到的编辑结果。

文字变化会重新排版；仅选区、焦点变化时复用不可变 TextLayout。已记录但尚未提交的布局句柄继续保留到帧被接受。

Windows 需真实 DirectWrite 命中查询。旧版 Wine 对 `HitTestTextPosition` 返回 `E_NOTIMPL`，只能用于其他接口与编译验收。中文输入法的候选窗口仍需人工使用本机 IME 验收。

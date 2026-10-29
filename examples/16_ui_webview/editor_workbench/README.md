# Writing desk

Xu SDK 0.10.11 原生 Input、TextArea、结构化菜单与显示器查询示例；模块版本 0.2.1。

从仓库根编译与运行（工具链要求见[父目录说明](../README.md#统一入口)）：

```sh
python3 scripts/ui_examples.py build editor_workbench
python3 scripts/ui_examples.py run editor_workbench
```

- 点击输入，Tab / Shift+Tab 切换焦点，单行 Enter 提交，多行 Enter 换行。
- 支持拖动选区、方向键、Home / End、Shift 扩展选区，以及系统复制、剪切、粘贴快捷键。多行超出视口后可滚动。
- Document 菜单提供重置、编辑开关、亮／暗主题、子菜单与禁用项；主修饰键 + Shift + R 重置，主修饰键 + Shift + D 查询显示器。
- 页脚显示真实屏幕查询结果。Windows 屏幕矩形使用物理像素；macOS / GTK 使用逻辑坐标。GTK 无法可靠获取主屏与工作区时，结果的 known 标记为 false。

`Editor` 保留控件值，聚焦时使用原生 `TextInput` 会话处理文字和 IME。`View.layout` 后调用 `place_editor`，使命中、绘制与输入法候选位置一致。原生文字事件与剪贴板请求结果都交给 `Editor.event`；键盘和指针交给 `widgets.dispatch_event`。剪切、粘贴只对请求时的会话版本生效，焦点切换或更新后会丢弃迟到的编辑结果。

文字变化会重新排版；仅选区、焦点变化时复用不可变 TextLayout。已记录但尚未提交的布局句柄继续保留到帧被接受。

Windows 需真实 DirectWrite 命中查询。旧版 Wine 对 `HitTestTextPosition` 返回 `E_NOTIMPL`，只能用于其他接口与编译验收。中文输入法的候选窗口仍需人工使用本机 IME 验收。

## XUUI 接入

此示例消费 [xu-ui-design](https://github.com/xulang-org/xu-ui-design) 的 `0.1.1-dev.1` 设计资源。`xuui_tokens/mod.xu` 由权威 Token JSON 生成，`design-source.json` 固定来源提交、Token 与适配器 SHA-256；保留该来源和 MIT 许可。

画布、文字、状态、输入边界、焦点、选择色与圆角使用同一份亮／暗 Token。Editor 改用 SDK `paint_styled`；主题变化更新 Styles 与背景，并标记所有相关节点重绘。Document → Dark appearance 切换主题；主题切换不重置输入内容。适配仅覆盖 Editor 与页面颜色，其他原生控件仍需对照 SDK 实际接口接入。

更新资源时，在固定的设计仓库提交执行 `npm run build`，复制 `dist/xu/xuui_tokens/mod.xu`，更新来源和校验值，并按仓库规则递增本模块版本。不得手工改生成的色值导致 Web 与原生分叉。

已在 macOS arm64、SDK 0.10.11 验证编译打包、英文标题提交、Tab 切换、多行中文粘贴及主题切换后的输入保留；Windows、Linux、中文 IME 候选位置和屏幕阅读器需各平台人工验收。当前自绘 Editor 未向本机辅助功能树提供可操作子控件，不能宣称已支持 VoiceOver；Token 接入不会补齐 SDK 语义接口。

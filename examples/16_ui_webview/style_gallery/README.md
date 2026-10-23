# 控件样式图库

需要 **Xu 0.10.1-dev.3**。使用 UI 开发分支构建的编译器：

```sh
XU_ROOT=/path/to/xu-ui python3 scripts/ui_examples.py check style_gallery
XU_ROOT=/path/to/xu-ui python3 scripts/ui_examples.py run style_gallery
```

切换深色、浅色、高对比度主题；把文字放大到 140%；切换紧凑密度。局部绿色按钮保留独立主题，并演示图片图标。检查悬停、按下、键盘焦点、勾选、禁用、错误和只读状态；输入框和多行编辑器支持原生 IME、选区、光标与预编辑下划线。滚动条支持拖动、轨道翻页、滚轮与键盘。

## 样式与 API

| API | 用法与边界 |
|---|---|
| `dark_styles()` / `light_styles()` / `contrast_styles()` | 创建完整 `Styles`；修改后 `validate()`，非法值抛出错误 |
| `Styles.font` / `text_scale` / `density` | 字体与文字缩放独立于几何密度；缩放和密度范围均为 0.5–4.0 |
| `ControlStyle` | 各控件独立的调色板、字体继承、内边距、圆角、边框、焦点、最小尺寸、对齐、图标和阴影 |
| `state_paint(when)` / `ControlStyle.states` | 组合状态规则；设置 `fields` 才覆盖相应字段，后匹配规则胜出，最多 32 条 |
| `StyledLabel` | 拥有标签文字与排版缓存；改色复用排版，字体变化重新排版，用完 `dispose()` |
| `Button.paint_icon_styled` | 图片保持纵横比；支持 leading / trailing / above / below，调用者持有 Image |
| `Editor.apply_styles` / `paint_styled` | 字体、行高、占位、选区、光标与预编辑；`set_read_only` 切换只读 |
| `Scroll.apply_styles` / `paint_scrollbar` | 调整条宽与滑块几何，在 `overlay_styled` 中绘制滚动条 |

状态位为 disabled=1、hovered=2、pressed=4、focused=8、selected=16、invalid=32、read_only=64。规则要求全部 `when` 位存在、全部 `unless` 位不存在；只覆盖 `paint_*` 掩码指定的字段。默认背景优先级为禁用、错误、只读、选中、按下、悬停、普通。

## 保留场景接入顺序

1. `View.set_styles` 设置全局主题；`set_style_scope` 为非根节点保存完整主题快照。后代继承到下一作用域，后续全局变化保留显式局部主题；`clear_style_scope` 恢复继承。
2. `set_style_measure` 注册依赖样式测量的节点。修改标签时 `StyledLabel.set_text`，再 `View.mark_measure`。
3. `layout_styled` 完成布局后调用 `place_editor`，同步画布全局位置和 IME；绘制、指针回调使用节点局部坐标。
4. `present_styled` 只在需要时提交帧；`overlay_styled` 在子节点之后执行。回调借用有效主题，不在回调里修改 View 或拓扑。
5. `set_capture_outside` 使滚动条拖动在视口外继续。关闭窗口时释放编辑器、标签、图片与 View；资源须保留到引用它们的帧被接受。

颜色与装饰变化只使重绘边界失效；字体与几何变化还会重新测量已注册节点。旧的八字段 `Theme` 与 `paint` 接口继续兼容。原生菜单和对话框跟随系统外观，WebView 内容使用 CSS；主题由应用显式切换。阴影采用至多八层近似。

只读编辑器保留选择、导航和复制，释放原生输入会话并拒绝写入及迟到 IME 提交。文本最多 16 KiB UTF-8，不含 NUL；大文本与大字体组合还受原生排版预算限制。

事件循环先排空已排队输入，再绘制一帧，避免快速输入逐键填满帧队列。Draw 默认不启用相机平移。桌面对象构建使用 UI 协程栈配置，支持原生字体排版调用。

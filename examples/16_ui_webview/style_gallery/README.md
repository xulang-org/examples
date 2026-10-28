# 控件样式图库

使用满足本目录 `xu.xun` 中 `xu:` 要求的 SDK 或配套开发编译器：

从仓库根执行，默认使用同级 `../xu`；可用 `XU_ROOT` 指定匹配的 Xu 源码目录：

从仓库根执行，默认使用同级 `../xu`；可用 `XU_ROOT` 指定匹配的 Xu 源码目录：

```sh
python3 scripts/ui_examples.py check style_gallery
python3 scripts/ui_examples.py build style_gallery
python3 scripts/ui_examples.py run style_gallery
```

切换深色、浅色、高对比度主题；把文字放大到 140%；切换紧凑密度。局部绿色按钮保留独立主题，并演示图片图标。检查悬停、按下、键盘焦点、勾选、禁用、错误和只读状态；输入框和多行编辑器支持原生 IME、选区、光标与预编辑下划线。滚动条支持拖动、轨道翻页、滚轮与键盘。

按钮和输入框使用约 150 ms 的状态过渡，按下反馈为 75 ms；开关圆钮滑动 180 ms；下拉面板展开 130 ms、关闭 95 ms，配合轻微位移和淡入淡出。过渡可以被新输入打断，不延迟点击或拖动。

鼠标移入可点击控件显示手形，文本区（含只读文本）显示 I 形，滑块显示水平方向指针。拖动时保留所属控件的指针，释放、移出或失焦时恢复。输入光标先保持可见，再柔和闪烁；输入、导航和选区变化会重新开始可见阶段，IME 预编辑时保持稳定。存在选区和只读状态时不显示插入光标。

## 样式与 API

| API | 用法与边界 |
|---|---|
| `dark_styles()` / `light_styles()` / `contrast_styles()` / `preset_styles()` | 创建完整 `Styles`；修改后 `validate()`，非法值抛出错误 |
| `StyleSwitcher` / `style_switcher()` | 统一管理主题预设、自定义命名主题、`text_scale` 与 `density`，通过 `apply(view)` / `cycle(view)` 一键切换 |
| `Styles.font` / `text_scale` / `density` | 字体与文字缩放独立于几何密度；缩放和密度范围均为 0.5–4.0 |
| `ControlStyle` / `set_style()` / `clear_style()` | 支持“全局 `Styles` + `View` 子树作用域 + 单控件实例 `set_style`”三级优先级覆盖 |
| `state_paint(when)` / `ControlStyle.states` | 组合状态规则；设置 `fields` 才覆盖相应字段，后匹配规则胜出，最多 32 条 |
| `StyledLabel` / `paint_box_styled` | 拥有标签文字与排版缓存，支持 `paint_styled` / `paint_text_styled` 与通用容器盒子绘制，用完 `dispose()` |
| `Button.paint_icon_styled` / `Checkbox.paint_styled` | 图标与复选框均支持 leading / trailing / above / below 排布及 `align_x` / `align_y` 对齐 |
| `Progress.measure_styled` / `paint_styled` | 支持基于 `min_width` / `min_height` / `padding` / `density` 的自动测量与内缩圆角进度填充 |
| `Switch` / `Radio` / `Slider` | 拨动开关、单选框、滑块（支持鼠标拖拽与键盘步进）；滑块的焦点圆环跟随圆形滑块。均支持 `Styles.switch_style()` / `radio_style()` / `slider_style()` 与单实例 `set_style` |
| `Tabs` / `segmented()` / `Select` | 分段控制器/标签栏与下拉选择框；点击正文或箭头展开 Auto / Manual / Locked，点击选项选择，Escape 或点击外部关闭；支持键盘导航 |
| `Divider` / `Badge` / `Card` | 水平/垂直分割线（支持带标题分割 `paint_labeled_styled`）、状态徽章（支持圆点指示器与四向排布）、带标题卡片容器 |
| `Stepper` / `ListBox` | 数值加减步进器与可滚动单选列表框；列表显示三行，滚轮滚动后可点击第四行 Controls，普通和紧凑密度共用绘制与点击几何；支持键盘翻页与 `ensure_visible` |
| `Editor.apply_styles` / `paint_styled` | 字体、行高、水平/垂直对齐、占位、选区、光标与预编辑；`set_read_only` 切换只读 |
| `Scroll.measure_styled` / `paint_scrollbar` | 调整条宽与滑块几何，在 `overlay_styled` 中绘制滚动条 |
| `Tween` / `PaintTransition` | 使用 `time.Instant` 与 `time.Duration`，显式单调时钟、四次 ease-out、连续改向与收敛；`style` 保留控件测量和内部文字配色，只覆盖动态绘制字段 |
| `View.set_cursor` / `pointer_cursor` | 每节点的指针语义、捕获期间保留、按 canvas generation 下发并去重 |
| `Editor.tick` / `next_caret_tick` | 明确推进插入光标时间；仅在闪烁边界和渐变阶段请求重绘 |
| `request_canvas_preferences` / `canvas_preferences_of` | 从原生宿主读取系统减少动态效果偏好；启动与重新获得焦点时刷新 |

状态位为 disabled=1、hovered=2、pressed=4、focused=8、selected=16、invalid=32、read_only=64。规则要求全部 `when` 位存在、全部 `unless` 位不存在；只覆盖 `paint_*` 掩码指定的字段。默认背景优先级为禁用、错误、只读、选中、按下、悬停、普通。

## 保留场景接入顺序

1. `View.set_styles` 设置全局主题；`set_style_scope` 为非根节点保存完整主题快照。后代继承到下一作用域，后续全局变化保留显式局部主题；`clear_style_scope` 恢复继承。
2. `set_style_measure` 注册依赖样式测量的节点。修改标签时 `StyledLabel.set_text`，再 `View.mark_measure`。
3. `layout_styled` 完成布局后调用 `place_editor`，同步画布全局位置和 IME；对 `Select`、`ListBox` 调用 `apply_styles`，同步输入所需的行高与内边距。绘制、指针回调使用节点局部坐标。
4. `present_styled` 只在需要时提交帧；`overlay_styled` 在子节点之后执行。回调借用有效主题，不在回调里修改 View 或拓扑。
5. 下拉面板在根节点的 `overlay_styled` 中绘制，使用 `paint_popup_box_styled` 和逐项 `paint_option_styled`；下方空间不足时用 `set_popup_above` 向上展开。示例先处理面板内的指针序列，再交给普通节点分发，选项点击不会落到背后的控件。
6. 使用 `tick` 推进尚未收敛的过渡，并只标记有变化的节点。闲置时等待输入；编辑器按 `next_caret_tick` 唤醒。系统要求减少动态效果时，颜色、开关和面板立即到达最终状态，插入光标保持稳定。
7. `set_capture_outside` 使滚动条拖动在视口外继续。关闭窗口时释放编辑器、标签、图片与 View；资源须保留到引用它们的帧被接受。

颜色与装饰变化只使重绘边界失效；字体与几何变化还会重新测量已注册节点。旧的八字段 `Theme` 与 `paint` 接口继续兼容。原生菜单和对话框跟随系统外观，WebView 内容使用 CSS；主题由应用显式切换。阴影采用至多八层近似。

只读编辑器保留选择、导航和复制，释放原生输入会话并拒绝写入及迟到 IME 提交。文本最多 16 KiB UTF-8，不含 NUL；大文本与大字体组合还受原生排版预算限制。

事件循环先排空已排队输入，再绘制一帧，避免快速输入逐键填满帧队列。Draw 默认不启用相机平移。桌面对象构建使用 UI 协程栈配置，支持原生字体排版调用。

日期和时间选择见 [date_time](../date_time/)；控件覆盖与尚未封装的能力见 [UI 示例说明](../README.md#控件与宿主能力边界)。

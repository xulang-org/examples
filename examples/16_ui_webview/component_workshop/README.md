# 原生组件工作坊

需要 Xu **0.10.10**。45 个命名入口涵盖全部基础、输入、数据、反馈与容器组件，并加入 Field、RadioGroup、ToastHost 和内嵌 WebView。全部示例调用公共组件和 Screen API。旧正式 SDK 0.10.7 缺少本示例所需的图表、菜单等 API，不能用于构建本示例。

```sh
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py check component_workshop
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run component_workshop
```

左侧选择组件；顶部切换亮色/暗色/高对比、常规/紧凑/触控、100%/140%/200% 字号。状态菜单测试普通、禁用、只读与错误；不适用时会明确说明。Hover、pressed、focus、selected 使用真实鼠标和键盘操作，Focus sample 把焦点交给预览。

工具操作随可用宽度和字号换行，并与预览一起滚动，避免大字号截断按钮文案。Focus sample 会将目标控件滚入可见区域。

Default/Custom 切换实例圆角、描边和间距，Default 恢复主题继承。Motion 切换过渡时间与曲线，Reduce motion 立即结束过渡。组件选项分别演示三态、提交型拖动、候选分组、筛选、全选、数据加载/错误、真实任务进度、通知与模态确认等。

- Tree 默认启用层级引导；筛选保留祖先，Reveal last 展开并定位末节点。Shift+F10 触发行操作。
- TaskButton 以 48 个实际计算批次报告进度；可以切换线条/填充/图标、失败重试和取消。任务 ID 与外观配置独立。
- Field 使用原生 Editor。软计数统计 Unicode scalar（组合 emoji 可占多个 scalar），不会截断文字或修改 IME 预编辑；业务决定校验时机。FieldArea 的最小/最大行数参与测量，示例为其安排足够高度，超出内容在 Editor 内滚动。
- Switch 的 Pending 由业务设置：等待期间保持焦点并阻止重复切换；成功/失败由业务明确完成或恢复，Reduce motion 下使用静态等待标记。
- Slider 提供刻度、吸附、数值标签与精确输入；ListBox 提供图标、主副文本、尾部信息和真实滚动条。
- Select 的 group heading 占一个索引，无法选中；disabled 项也不能选中，键盘会跳过。索引包含分组行。
- Table 支持逐列最小宽度，拖动不能越过最小值；Pagination 将精确页码输入与首页/末页操作组合。
- Card 的三个内容槽由 Screen 布局；静态容器中的操作由独立按钮承担。
- Scroll 的子控件使用内容坐标。画廊预览本身也是 Scroll，字号放大后可以滚动。Focus last 自动滚入最后一个按钮。
- StyledLabel 展示换行、按原生排版 cluster 省略，以及完整文字提示。完整 text 保留在控件中。
- WebView 使用真实系统子视图；隐藏恢复保留页面内容，Ctrl+Tab 回到 Canvas 焦点链。网页 CSS 由网页自身管理。

WebView 需要链接对应宿主插件，构建时不能使用 `--no-webview`。统一启动器会保留 WebView 插件。可见的原生网页不参加 Canvas 离屏截图；与 Canvas 浮层重叠时临时隐藏，关闭浮层后恢复。

组件的 hover 和选中动画只在状态变化期间请求帧；拖动直接跟随指针。缩放与密度是控件的真实配置，不使用画布整体缩放来伪装适配。文本编辑的撤销/重做历史、高级虚拟化、拖放及系统无障碍桥接尚未实现。ButtonGroup 和 Toast 的 Undo 是应用动作示例，不代表文本编辑历史。


Badge 页面使用真实组合场景：按钮右上角的未读数、头像边缘的在线点、工作区行的状态标签。`+1`、清零和 `99+` 操作展示实际数值变化；点击收件箱清零，点击头像切换在线状态。

```xu
inbox := screen.button("Inbox 收件箱") or >>
unread := screen.badge_on(inbox,"12",widgets.BADGE_COUNT) or >>
screen.set_badge_tone(unread,widgets.BADGE_ERROR) or >>
screen.set_badge_count(unread,123,99,false) or >>
```

`badge_on` 自动测量并随锚点布局、缩放、滚动和显隐。`set_badge_anchor(id,anchor,x,y)` 可选择锚点上的相对位置；角标必须在锚点之后创建。独立状态标签使用 `badge_size` 的内容尺寸。`BADGE_STATUS` 保持原有标签样式，`BADGE_COUNT` 是紧凑实色计数，`BADGE_DOT` 是状态点。在线点旁仍保留文字，避免只凭颜色传递状态。

左下角的语言菜单切换组件默认文案与格式：中文、English 或 Custom。Custom 展示自定义日期占位/星期标题/日期顺序和小数分隔符。应用传入的标题、选项和验证文案由应用自己翻译。

```xu
mut locale := widgets.locale_zh_cn()
locale.set_text(widgets.TEXT_TODAY,"回到今天") or >>
locale.set_first_weekday(0) or >>
screen.set_locale(&locale) or >>
screen.set_control_locale(date_id,&widgets.locale_en_us()) or >>
// 恢复整屏语言继承：
screen.clear_control_locale(date_id) or >>
```

Locale 可配置日期模板、月份与星期名称、周起始、12/24 小时和时段位置、数字/小数/分组符号以及组件内置文案。切换时保持用户值、选择、任务 ID 与进度。TaskButton 的显式 `set_labels` 业务文案保留；TimePicker 的显式时制也保留，低层 `use_locale_clock` 恢复语言的时制。默认 `locale_default` 保留英文/年月日/周一/24 小时，`locale_en_us` 使用月日年/周日/12 小时。数字配置用于显示，业务输入解析、货币、复杂复数和 RTL 布局由应用或后续能力负责。

Pagination 现在同时演示数据总量、每页条数、范围摘要、紧凑跳页与 Enter/Go、空/满切换和内联校验。`screen.set_pagination_data(id,total,page_size)` 保留原来首条目所在位置；`pagination_data` 返回实际当前页，`pagination_summary` 返回本地化范围摘要。原有零开始的页索引仍适用。

Popover 演示真实偏好表单。子控件通过 `set_parent(child,popover)` 放进浮层；坐标相对面板。取消/外点/Escape 不提交，Apply 更新结果。打开进入首个可用控件，关闭返回触发器；Tab 移出时关闭这个非模态浮层。`set_popover_placement` 配置上下左右、对齐、间距、偏移与翻转，`set_popover_dismissal` 配置外点/Escape。

Tooltip 是被动帮助。Select、ComboBox、MultiSelect 等候选列表展开时，Screen 自动隐藏提示；列表关闭后重新等待 Tooltip 的 delay，避免遮挡选项或立即闪回。Popover、Modal 打开时暂停背景控件的提示，浮层内部控件仍可显示自己的帮助。直接使用 Tooltip 时可通过 `set_blocked(blocked, now)` 接入相同规则。

新动效包含分页逐项悬停与选中反馈、按钮松开恢复节奏、按剩余距离缩短的反向切换，以及交互 Popover 的整体透明合成。`MotionSpec.curve` 支持 linear、out quart/quint、in quart/quint、in-out cubic。减少动效下直接完成，空闲后不持续请求帧。原生 WebView 不参加 Canvas 透明合成。


ButtonGroup 展示独立操作、单选和多选三种语义，不能用 Segmented 代替操作按钮组。每项有稳定 key、独立禁用和按钮变体；支持横竖、内容宽/等宽、拼接/间距。Tab 进入一次，方向键跳过禁用项，Home/End 定位，Enter 或空格在松开时激活。方向键只移动焦点，不自动触发操作。应用通过左下角语言菜单更新组内文案，保留选中值和逐项禁用。

```xu
items := [widgets.button_group_item(10,"Undo"),widgets.button_group_item(20,"Redo"),widgets.button_group_item(30,"Save")]
group := screen.button_group(&items,widgets.BUTTON_GROUP_ACTIONS) or >>
screen.set_button_group_item_enabled(group,20,false) or >>
screen.place(group,24.0,24.0,420.0,40.0) or >>
```

独立操作发 ACTION_ACTIVATE，单选/多选发 ACTION_CHANGE，Action.value 为 key。button_group_values 返回选中的 key；set_button_group_checked 设置受控值；set_button_group_label 只改文案。ButtonGroupAppearance 控制布局，set_motion 和 set_control_style 与其他控件一致。

Chart 通过自己的原生 Canvas 绘制折线、面积、分组柱状、散点和环形，不使用 ECharts 引擎。Data 更新会按稳定 series/point ID 接续当前显示值；支持 hover 提示、点选、图例显隐、十字线、Ctrl/Command + 滚轮缩放、拖动平移和双击/Home/0 复位。普通滚轮留给外层 Scroll。方向键检查数据，Enter 选中，Escape 清理。只读保留数据检查，阻止改变选择与视窗。

```xu
chart := screen.chart() or >>
mut series := widgets.chart_series(1,"Build",widgets.CHART_LINE)
series.smooth = true
series.points = [widgets.chart_point(101,1.0,18.0),widgets.chart_point(102,2.0,25.0),widgets.chart_point(103,3.0,21.0)]
screen.set_chart_series(chart,&[series]) or >>
screen.place(chart,24.0,80.0,780.0,360.0) or >>
mut appearance := widgets.chart_appearance()
appearance.y_unit = " ms"
screen.set_chart_appearance(chart,&appearance) or >>
```

ChartAppearance 配置网格、图例、提示、缩放/平移、数字精度、单位、空文案、调色板、命中半径、柱宽、面积透明度和环形内径。系列可配置颜色、线宽、点半径和平滑曲线。容器和字体遵循 ControlStyle；数值与空文案遵循 Locale，业务系列名与数据点标签由应用提供。

ACTION_CHART_HOVER/SELECT/VIEWPORT/LEGEND 区分交互；chart_hovered、chart_selection 和 chart_viewport 返回详细数据。chart_position 与 chart_hit_test 供业务侧组合标注。set_chart_domain 固定数值坐标域，use_chart_auto_domain 恢复自动域；set_chart_viewport 接收 0..1 归一化范围。pause/resume/seek_chart_animation 提供数据过渡控制。

首批限制为 8 系列、共 4096 个有效数据点；更新额外保留最多同等数量的退出点。ID 为正 32 位整数。折线/面积/柱状要求 x 严格递增，散点允许任意顺序；环形只接收一个非负值系列。当前没有 ECharts 配置兼容、多轴、类别/时间/对数轴、堆叠、地图、brush 或海量采样。

AnimationController 为应用自定义绘制提供显式时钟、播放/暂停/续播/反向/进度定位/取消/完成、延迟与重复。animation_options 配置时长、曲线、速率、次数和交替方向；iterations=0 表示循环。调用方用 next_tick 安排唤醒，并将 value() 用于自己的 Frame 绘制；暂停与静止不持续请求帧，减少动效结束循环。Screen 图表的数据过渡采用单次正向动画。


## 区间、分栏与菜单

RangeSlider 通过 `screen.range_slider(min, max, lower, upper)` 创建。`set_range_options` 配置 step、min_distance、RANGE_CLAMP/RANGE_PUSH、vertical、reversed、drag_range 和 commit_only；`set_range_appearance` 配置刻度、数值与精度。Tab 分别访问两个端点；方向键、Home/End、Page Up/Down 调整当前端点，空格切换端点，Escape 取消未完成编辑。`range_values` 返回 lower/upper/active。普通模式发预览 ACTION_CHANGE，释放指针或对应按键发 ACTION_COMMIT；commit_only 只发提交事件，取消恢复原值。整段拖动保持区间宽度，终点限制优先于步进。

SplitView 通过 `screen.split_view(ratio)` 创建，用 `split_pane(id, 0/1)` 获取稳定的子容器 ID，再把业务内容 parent 到对应容器。`split_layout` 返回当前局部尺寸，应用据此布局子控件；子控件的绘制和命中都限制在所属面板。`set_split_options` 配置方向、两侧最小尺寸、分隔条大小和键盘步长；窗口太小时按两侧最小尺寸比例分配可用空间。拖动或方向键调整，Shift 加速，Escape 恢复编辑前比例；双击或 Enter 复位/展开。`set_split_collapsed` 保留比例，隐藏面板的焦点返回分隔条。应用用 `split_layout().ratio` 和 `set_split_ratio` 保存/恢复自己的布局，不由组件写入存储。

Menu、ContextMenu、SplitButton 共用 `screen.menu(label, items, mode, primary_key)`。mode 为 MENU_DROPDOWN、MENU_CONTEXT、MENU_SPLIT；SplitButton 的 primary_key 必须为正数，主按钮与展开箭头独立操作。MenuItem 的 key 是稳定的动作 ID，parent 指向 MENU_SUBMENU 项；支持 MENU_ACTION、MENU_CHECK、MENU_RADIO、MENU_SEPARATOR、MENU_HEADING。相同 parent 和 radio_group 的单选项互斥。快捷键的逻辑键、修饰键和展示文案由应用提供；禁用父菜单会禁用其后代。

菜单支持指针、右键、Shift+F10、方向键、Home/End、Enter/空格、Escape、滚轮，子菜单和根菜单按视口翻转并裁剪；内容过高时显示滚动位置。Escape 先退出子菜单，再关闭根菜单；Tab 移出会关闭菜单。当前没有原生触摸长按识别。菜单保留自己的触发器焦点，打开候选菜单时背景 Tooltip 暂停。

`set_menu_item_label` 和 `set_menu_label` 只更新文案，保留勾选状态，适用于语言切换；`menu_checked`、`set_menu_checked`、`set_menu_item_enabled` 和 `set_menu_items` 管理状态与数据。ACTION_ACTIVATE 表示普通动作/主按钮，ACTION_CHANGE 表示勾选或单选，Action.value 是 key。MenuAppearance 控制行高、宽度、hover delay 和是否选择后关闭，ControlStyle/MotionSpec 控制实例样式与反馈节奏。RangeSlider 的数字遵循 Locale；菜单业务文案由应用完整提供。


菜单测量缓存普通宽度数据，只保留当前可见行的原生文字布局；滚动会释放屏幕外的缓存，绘制帧独立保留已记录的布局。动态文字登记没有 1024 个对象的固定配额，原生字体与绘图仍遵守宿主的字节与坐标预算。菜单的短距离反向开合按剩余距离缩短时长，键盘按下反馈与指针反馈使用相同 MotionSpec。


## 表格排序

Table 默认在表头点击、Enter/空格释放时执行稳定的本地排序，同时沿用 ACTION_ACTIVATE 通知排序列；按住按键不会重复切换。再次激活同列在升序/降序之间切换。`set_table_sort_kind(id, column, TABLE_SORT_TEXT/TABLE_SORT_NUMBER)` 指定列类型：文本按 UTF-8 字节序（区分大小写）；数值按有限 f64 解析规范 ASCII 数字，空白、NaN、Infinity 和无效值排在末尾，同值保持原相对顺序。需服务器排序、特定语言排序规则或业务格式解析时，`set_table_local_sort(id, false)` 保留标记和事件，由数据源控制行序；component_catalog 演示这种模式。

`set_cell`/`table_cell` 仍按源行索引读写数据，排序只修改显示映射；`table_view_cell` 读取显示行，`table_row_at` 返回显示行对应的源行，`table_view_row` 做反向查询。选中行在排序后跟随同一源行；`table_selected_row` 返回该源行，原有 `selected` 返回显示行。`set_table_selected_row` 按源行选中。源行身份只在当前数据集及行数范围内成立，不代替业务主键。

`sort_table(id, column, descending)` 供程序排序，column=-1 恢复源行顺序。排序列上的 `set_cell` 更新会刷新本地排序，源数据索引不变；删掉选中源行时清除选择。手动/远程排序的业务身份由应用维护。只读或禁用时不响应用户排序；程序仍可更新数据与排序。工作坊提供乱序组件名和 2/7/14/120/210 等数值，可直接比较文本与数值排序，并保留当前选中源行。

## 图标与文字按钮

`Button` 页首先展示图标＋文字按钮，覆盖图标在文字左、右、上、下四种布局。按钮按实际内容宽度排列，空间不足时换行；每个图标按钮都有独立的尺寸 Stepper，可在 12–64 px 间调节，修改一个不会影响其他按钮，上下布局的高度随自己的图标增长。尺寸会随全局字号和密度缩放。禁用状态只作用于示例按钮，仍可分别调节尺寸。`IconButton` 页单独展示纯图标按钮与动作提示。两者都支持键盘激活、禁用状态和主题/字号切换。

```xu
id := screen.button_with_icon("logo.png", "Build project") or >>
mut appearance := styles.button_variant(widgets.BUTTON_PRIMARY) or >>
appearance.icon_placement = widgets.ICON_LEADING
appearance.icon_size = 24.0 * styles.text_scale
screen.set_control_style(id, &appearance) or >>
size := screen.measure_button(id) or >>
screen.place(id, 24.0, 24.0, size.width, size.height) or >>
```

图片随 `www/logo.png` 嵌入，Screen 持有并在销毁时释放；打包时保留 `--www` 资源目录。


## Tooltip 定位与触发

`Tooltip` 页可切换上、下、左、右定位，并在窗口边缘自动翻转。提示默认在锚点上方居中；滚动时只对可见的锚点部分定位，完全滚出裁剪区域时隐藏。鼠标悬停 500 ms 显示，键盘 Tab 或显式 `focus()` 立即显示；鼠标点击不会被当作键盘焦点。点击或滚轮操作收起提示，移出后可再次触发；Escape 抑制当前这一轮提示，离开并重新进入或重新获得焦点后恢复。

```xu
mut placement := widgets.overlay_placement()
placement.side = widgets.OVERLAY_RIGHT
placement.align = widgets.OVERLAY_CENTER
screen.set_tooltip_placement(tooltip_id, &placement) or >>
```

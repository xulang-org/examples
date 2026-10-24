# 原生组件目录

需要 **Xu 0.10.2-dev.1+**。本示例把 12 种新增组件放在同一应用中，演示数据、输入与浮层如何共享主题、文字缩放、密度和事件状态。全部使用 Xu 原生 Canvas 绘制；日期与时间控件另见 [date_time](../date_time/)。

从 examples 仓库根运行，`XU_ROOT` 指向包含本批 API 的 Xu 源码目录：

```sh
XU_ROOT=/path/to/xu-ui python3 scripts/ui_examples.py check component_catalog
XU_ROOT=/path/to/xu-ui python3 scripts/ui_examples.py build component_catalog
XU_ROOT=/path/to/xu-ui python3 scripts/ui_examples.py run component_catalog
# 已完成打包后再次打开：
XU_ROOT=/path/to/xu-ui python3 scripts/ui_examples.py run component_catalog --no-build
```

产物在 `build/ui/<target>/component_catalog`，macOS 使用 `.app`，Windows 使用 `.exe`。`build --target linux-arm64` 或 `--target windows-x64` 需要对应工具链；`run` 只运行本机目标。首次运行不需要网页、HTTP 服务或下载示例数据。

## 操作与能力

Data、Inputs、Overlays 切换三组示例，Theme、Text size、Density 切换浅色/深色/高对比度、100%/140% 文字及普通/紧凑密度。Tab / Shift+Tab 切换焦点；Escape 关闭当前接收该键的浮层。数据和选项都是应用中的固定示例值。

| 组件 | 可操作能力 | 当前边界 |
|---|---|---|
| `Table` | 点击行选择；点击表头请求升降序；键盘导航、滚轮、固定表头 | 等宽列、单选、单行文字；排序与取数由应用执行，无单元格编辑、列拖动或无界数据源 |
| `TreeView` | 点击箭头展开/收起；方向键在父子及可见节点之间移动；滚轮 | 有限树、单选；无拖拽、多选、异步加载或单节点删除 |
| `Pagination` | 页码、前后按钮、Home/End、方向键；切页后更新示例表格 | 数据获取由应用执行，无内置网络请求或页大小选择器 |
| `ComboBox` | 原生文字编辑与 IME、输入过滤候选、箭头展开、键盘选择 | 1–64 个固定单行选项；支持自由输入，不是异步搜索或完整自动补全引擎 |
| `MultiSelect` | 多项独立勾选、键盘移动、Enter/Space 切换、滚轮 | 1–64 个固定选项；返回选中索引，无自由创建标签或分组搜索 |
| `ColorPicker` | R/G/B/A 四个 0–255 通道，拖动滑块、键盘调整、透明度棋盘预览 | 内嵌 RGBA 编辑器；无 HSV 色盘、屏幕取色、HEX 文本框或色彩配置管理 |
| `Breadcrumbs` | 点击祖先、方向键选择祖先、Enter/Space 激活、溢出裁剪 | 1–32 段固定路径；当前末段不可点击，跳转由应用处理，无折叠菜单 |
| `Spinner` | 可启停、按时间更新；减少动效时显示静态指示 | 表示不确定等待，不计算完成百分比，也不管理后台任务 |
| `Popover` | 锚定触发器、下方/上方放置、约束到视口、Escape/外点关闭 | 仅管理浮层表面、几何与状态；内容、焦点和动作由应用组合 |
| `Tooltip` | hover 延时、键盘 focus 即时显示、Escape 隐藏与抑制重现 | 文本由调用方标签提供，不承载可交互按钮或富内容 |
| `Modal` | 遮罩、关闭按钮、Escape、Tab 循环、关闭后恢复焦点 | 应用必须把焦点应用到真实子控件并拦截背景输入；无内置表单或系统对话框语义 |
| `Toast` | 定时关闭、关闭按钮、hover/focus 暂停、可中断淡入淡出 | 单条通知状态；堆叠、队列、公告语义和动作按钮由应用管理 |

Table 使用 6 行示例记录。表头变化触发应用重新填充单元格；页码变化给记录附加页码。TreeView 使用固定的 Workspace 树。Inputs 页展示组合框、多选、RGBA 和运行中的 Spinner。Overlays 页展示锚点浮层、两按钮 Modal、三秒 Toast 与 Tooltip。

## 数据与输入 API

有文字资源的组件在 `canvas_ready` 后用匹配的 `draw.Context` 和 `ui.CanvasEvent` 创建。典型构造方式：

```xu
headers := ["Name","Status"]
mut table := widgets.table(&context,&identity,id,&bounds,&headers,rows,selected) or >>
mut tree := widgets.tree_view(&context,&identity,id,&bounds) or >>
mut pager := widgets.pagination(&context,&identity,id,&bounds,pages,current) or >>
mut combo := widgets.combo_box(&context,&identity,id,&bounds,&options,&value) or >>
mut multi := widgets.multi_select(&context,&identity,id,&bounds,&options) or >>
mut color := widgets.color_picker(&context,&identity,id,&bounds,draw.rgba(59,130,246,220)) or >>
mut crumbs := widgets.breadcrumbs(&context,&identity,id,&bounds,&path) or >>
mut spinner := widgets.spinner(&bounds) or >>
```

所有索引从 0 起，分页标签显示从 1 起。Table 最多 16 列、512 行，总单元格不超过 768；无选中行使用 `-1`。`set_cell` 更新文字，`sort_column` / `sort_descending` 表示排序请求，应用自行按数据类型重排记录。组件选中值是行索引，业务 ID 由应用维护。滚动条当前用于指示位置，滚动通过滚轮和键盘完成。

TreeView 最多 512 个节点，深度 0–32。`add(parent,&text)` 的根父级为 `-1`，返回稳定节点索引，直到 `clear`。`selected` 是节点索引；`visible_node(row)` 将可见行映射回节点。`set_selected` 会展开祖先；收起祖先导致选中节点隐藏时，选择移到该祖先。

Pagination 支持 0–1,000,000,000 页，空页集合使用 `current=-1`；最多显示 7 个连续页码及前后按钮。`set_selected`、`set_page_count` 管理页码，`action_change.value` 是从 0 起的页码。

ComboBox 的 `value()` 是文本，`selected()` 是匹配选项的原索引，自由输入未匹配时为 `-1`。过滤基于已提交文本；原生 IME 预编辑期间继续把文字会话事件交给 `event()`，不能只派发 key 事件。显式展开显示全部选项，编辑后再过滤。MultiSelect 的 `values()` 返回选中选项的原索引数组，`set_selected(index,bool)` 控制各项。两个候选面板最多显示 8 行并支持滚轮；`set_popup_above` 由宿主决定向上展示，尚未自动适配任意视口。

ColorPicker 的 `value()` 返回 `draw.Color`，`set_channel(index,value)` 对应 R/G/B/A。Up/Down 切换通道，Left/Right、Home/End、Page Up/Page Down 调整当前通道。Breadcrumbs 激活时返回 `action_activate`，`value` 是祖先索引；应用据此执行跳转。

样式或布局变化后，先 `set_bounds`、`apply_styles(&styles)`，再处理输入。文字大小、行高、密度、绘制与命中共享同一指标；每帧重复设置同一矩形不会清除正在进行的按下状态。候选面板用 `paint_popup_styled` 在普通内容之后绘制，并在底层控件之前接收输入。

## 浮层、时间和焦点

浮层所有矩形使用完整 Canvas 的逻辑像素坐标；采用 View 局部坐标的宿主须先转换。Popover 与 Tooltip 的 `anchor`、目标 `Size`、`viewport` 共同决定位置，空间不足时向上翻转，最终尺寸与位置夹在视口内。普通 Popover 内容由应用自行绘制和裁剪。

```xu
mut pop := widgets.popover(&identity,id,&anchor,&size,&viewport) or >>
mut hint := widgets.tooltip(&identity,id,&anchor,&size,&viewport) or >>
mut modal := widgets.modal(&identity,id,&panel,&viewport) or >>
mut toast := widgets.toast(&identity,id,&bounds) or >>
now := time.instant()
pop.open(now,reduced_motion) or >>
toast.show(now,time.seconds(3) or >>,reduced_motion) or >>
```

时长使用 `time.Duration`，动效和期限使用进程内单调 `time.Instant`；日历值 `time.Time` 用于日期时间控件。Tooltip 默认 hover 500ms，键盘 focus 立即显示；`set_delay(Duration)` 调整延时。Escape 隐藏后，在本次 hover/focus 都结束前不会重现。Toast 的零时长表示持续显示，hover 或 focus 暂停期限，`dismiss` 手动关闭；Toast 消费自身可见区域内的指针输入，包括关闭淡出期间。

组件没有私有轮询线程。宿主调用 `tick(now,reduced_motion)` 并按 `next_tick(now)` 返回的最近 deadline 唤醒；零 deadline 表示不需要定时唤醒。Spinner 的 `next_tick` 另接收 `reduced_motion`，ComboBox 通过 `next_caret_tick` 提供光标唤醒时间。减少动效时浮层直接到目标状态，Spinner 保留静态提示。输入后创建期限必须使用当前 Instant，不能沿用等待输入前的旧时刻。

Modal 的焦点与输入所有权需要宿主接入：

1. 打开前取消背景控件的指针按下、捕获与原生文本焦点。保留 View 的应用可调用 `widgets.cancel_input(view,handler)`。
2. `set_focus_order(&ids)` 注册 Modal 内可用子控件，`open(now,reduced_motion,restore_id)` 记录背景焦点。将 `focused_id()` 应用到实际 Button/Editor，鼠标点选时通过 `set_content_focus(id)` 同步。
3. 使用 `blocks_input()` 作为背景门禁，它包含关闭淡出期间。先调用 `modal.input` 并处理 `Action.consumed`，再由 `allows_content(event)` 决定是否转交 Modal 子树。即使事件已关闭 Modal，也不能再把该事件交给背景。
4. 每次 tick 后调用 `take_restore_focus()`；只有完全隐藏后才返回一次恢复目标。`focus_restore_id()` 仅供查询。

Modal 默认外点消费但不关闭；`set_dismiss_on_outside(true)` 可改为外点关闭。宿主在面板后绘制子控件并裁剪到 `bounds()`，需要时把 `opacity()` 应用到子内容。Modal 管理循环顺序，应用管理真实控件；Canvas 绘制没有因此自动获得系统原生 accessibility 节点。

## 生命周期与集成边界

`window_id`、`canvas_id` 和 generation 一起确定输入及资源的归属；普通输入忽略不匹配的代次。窗口失焦、控件移除与 Canvas 关闭时，宿主应转发对应取消/生命周期事件。Tooltip/Popover 的 scoped `route_cancel` 清除 hover、pending 与打开状态；Toast 的取消清除交互并恢复剩余期限。Modal 失焦时应取消子控件按下状态，重新聚焦时再同步其焦点顺序。

Table、TreeView、Pagination、ComboBox、MultiSelect、ColorPicker、Breadcrumbs 持有原生文字资源，用完调用 `dispose()`。Popover、Modal、Tooltip、Toast 不拥有传入的标签；`StyledLabel` 仍由调用方释放。Frame 接受绘制前不要释放其中引用的资源。`needs_paint`、`painted`、`Action.needs_paint` 和各组件的绘制方法共同支持按需重绘，不需要空闲时持续绘制整个窗口。

这些组件覆盖常用基础交互。高级数据表、富文本编辑、拖拽工作区、完整本地化与辅助技术桥接仍需后续独立设计，示例也没有连接真实服务或后台任务。

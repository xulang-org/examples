# 日期与时间控件

需要满足本目录 `xu.xun` 中 `xu:` 要求的 SDK。

从仓库根执行，默认使用同级 `../xu`；可用 `XU_ROOT` 指定匹配的 Xu 源码目录：

```sh
python3 scripts/ui_examples.py check date_time
python3 scripts/ui_examples.py build date_time
python3 scripts/ui_examples.py run date_time
```

`DatePicker` 与 `TimePicker` 的值都是 `time.Time`。日期控件提供周一为首日的月历、上/下月按钮、日期范围和键盘导航；时间控件提供 24 小时制的时、分、秒分段输入与加减按钮。日期和时间同步显示为 RFC3339。初始值固定为闰日 `2024-02-29T14:35:20.123Z`，便于检查跨月行为。

- Tab / Shift+Tab 切换日期与时间焦点。
- 日期：点击展开；方向键移动一天或一周，Page Up / Page Down 翻月，Home / End 移到本周首尾，Enter 选择，Escape 关闭。
- 时间：点击选择分段；左右键或冒号切换分段，数字输入，Up / Down 加减，Home / End 取该分段的首尾值。两位数字后移到下一分段。
- T 切换深色、浅色、高对比度，F 切换文字大小，D 切换密度。

控件支持单实例 `set_style`、禁用、只读、错误状态与 `route` 焦点分发。`DatePicker.set_range` 限制可选日期并夹取现有值；`set_popup_above` 改为向上展开。样式改变后调用 `apply_styles`，使绘制和点击共用同一套月历几何。月历由 `paint_popup_styled` 在普通内容之后绘制；应用应优先把面板范围内的输入交给它，再分发给底下的控件。示例采用窗口坐标直接路由，保留场景可用 `route` 接入。

`TimePicker.set_show_seconds(false)` 切为时/分模式，隐藏的秒值仍保留。每个分段在自己的范围内循环，不把 23 点加一解释为第二天。改日期保留时分秒和纳秒，改时间保留日期和纳秒。控件按 `Time` 的 UTC 访问器解释字段，不暗中使用系统时区；时区选择、日期区间和本地化格式尚未封装为控件。

## 时间 API 的三种含义

| 类型 | 作用 | 典型 API |
|---|---|---|
| `time.Time` | 日历和 Unix 时间，可保存及格式化 | `date`、`now_time`、`parse_rfc3339`、`format_rfc3339`、`add_date` |
| `time.Instant` | 进程内单调时钟读数，用于动效与截止时间 | `instant`、`add(Duration)`、`since(Instant)`、`elapsed` |
| `time.Duration` | 有正负的时长，精度为纳秒 | `milliseconds`、`seconds`、`add`、`sub`、`sleep`、`timer`、`ticker` |

```xu
delay := time.milliseconds(180) or >>
start := time.instant()
deadline := start.add(delay) or >>
elapsed := time.instant().since(start)
println(elapsed.format())
_ = deadline
```

`Duration` 的单位转换、加减及解析会检查整数溢出；`Instant` 不能传给日历控件，也不能格式化为日期。`instant_from_ns` 用于同一单调时钟来源的原生事件或确定性测试，不用于 Unix 时间。`zero_instant` 在 UI 调度中表示未设置的截止时间。

原有 `time.now()` / `time.since()` / `time.sleep(int)`、单数单位函数以及 `new_timer(int)` / `new_ticker(int)` 继续兼容。它们的时长和单调读数使用纳秒 `int`；新增代码可使用有类型的接口，避免混淆毫秒、纳秒和日历时间。

# XACP Screen adapter

This native UI example demonstrates the in-process XACP adapter for
`widgets.Screen`: bounded tree inspection, an owned screenshot `Surface`, and
an authorized focus action. It does not open a network listener or expose a
remote control endpoint.

Run from the examples repository root:

```sh
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py check xacp_screen
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run xacp_screen
```

The adapter session explicitly receives only `PERM_UI_READ` and
`PERM_UI_MUTATE`. Screenshot surfaces are native resources and must be
disposed, including on error paths. The returned tree is a bounded copy;
control handles remain local to the live screen.

事件通过 `ActionDispatcher` 按控件 ID 注册（`on` / `on_any`），在循环中 `dispatch`。
请使用包含该 API 的当前 Xu 源码/SDK；样式配置在独立样式路径处理。

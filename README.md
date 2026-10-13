# Xu 示例

语言教程、可运行示例与桌面 UI 示例的独立仓库。

默认与语言仓库 `xu` 放在同级目录。使用当前产品编译器；开发规则见
`../xu/docs/DEVELOPMENT.md`（本机文档）。

```sh
make run EXAMPLE=01_hello
make check EXAMPLE=17_database_postgres
XU_ROOT=/path/to/xu make run EXAMPLE=01_hello
```

`XU_ROOT` 选择语言仓库；`XU` 可显式选择 hosted 产品编译器。
驱动示例的清单通过 `file:../../../packages` 引用同级 packages 根模块；
若改变目录布局，应相应调整这些显式依赖路径。
输出放在忽略的 `build/`，不提交编译器或运行产物。

- [示例索引](examples/README.md)
- [语言使用帮助](USAGE.md)
- [语言特色](FEATURES.md)

UI 打包器和 HTTP 部署工具仍属于语言产品工具，可从 `$XU_ROOT/scripts/`
调用，并用 `XU_EXAMPLES_ROOT` 或显式 `--src` / `--source` 选择本仓库内容。

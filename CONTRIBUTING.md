# 参与 XU examples 开发

请遵守 [XU GitHub 仓库管理规范](https://github.com/xulang-org/xulang-org/blob/main/.github/REPOSITORY_RULES.md)。日常任务从 `dev` 建分支，通过 PR 合回；正式交付进入 `main`。后台门禁须按 [配置清单](https://github.com/xulang-org/xulang-org/blob/main/.github/GITHUB_SETTINGS.md) 单独启用。

修改示例的源码、资源或模块内说明时，递增该示例最近模块根 `xu.xun` 的 `version:`；`xu:` 记录实际验收的 SDK。两者独立，根 `VERSION` 也不能代替示例模块版本。仅修改根贡献指南或 `.github/` 模板不修改示例模块版本。

## 修改与验证

从仓库根运行，`EXAMPLE` 是 `examples/` 下的相对目录。默认用相邻 `../xu` 的产品编译器，可用 `XU_ROOT` 指定实际 checkout：

```sh
make check EXAMPLE=01_hello
make run EXAMPLE=01_hello
make build EXAMPLE=01_hello
```

将 `01_hello` 换为实际修改的示例；检查输出与模块 README 一致。网络、数据库、UI 和 WebAssembly 示例还须按对应 README 准备服务、宿主依赖或 Node.js。仅修改仓库规则和模板时检查文档、链接、表单和模板一致性即可。

## 提交 PR

说明示例的学习目的、SDK 和平台、运行命令、预期与实际结果、模块版本变化及文档更新。保留容易复现的最小示例，不混入私有凭据或真实服务数据。

`examples-ui` 等本机工作树仍属于本仓库；已有改动与其他任务的模块版本须协调后保留，不能覆盖。构建产物与本机研发记录保持忽略。

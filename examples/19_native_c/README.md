# C 静态模块

这个示例把 `bridge.c` 编译为 XUNATIVE archive，再把多个目标 slice 打进 `native/native_add.xulib`。[`xu.xun`](xu.xun) 通过 `native:` 声明模块，应用不需要在每次构建时传入静态库路径。

单目标 archive 同时包含：

- 合法的 Mach-O/ELF/COFF `xu.o`，其专用 section 保存 Xu 导出签名；
- 编译器 生成的 C 目标文件，供系统 linker 静态链接。

macOS arm64/x64 多目标构建：

```bash
python3 ../xu/scripts/xunative.py compile \
 --manifest examples/19_native_c/native.json \
 --target macos-arm64 \
 examples/19_native_c/bridge.c \
 -o examples/19_native_c/native/native_add-arm64.a

python3 ../xu/scripts/xunative.py compile \
 --manifest examples/19_native_c/native.json \
 --target macos-x64 \
 examples/19_native_c/bridge.c \
 -o examples/19_native_c/native/native_add-x64.a

python3 ../xu/scripts/xunative.py pack \
 --manifest examples/19_native_c/native.json \
 --slice macos-arm64=examples/19_native_c/native/native_add-arm64.a \
 --slice macos-x64=examples/19_native_c/native/native_add-x64.a \
 -o examples/19_native_c/native/native_add.xulib

../xu/scripts/run_product_compiler.sh --cwd "$PWD" "$(pwd)/../xu/bin/xu" build examples/19_native_c \
 -o /tmp/native_add
/tmp/native_add.xin
```

最后输出 `42`。`xu build` 会读取项目以及已加载外部 `require` 包自己的 `xu.xun native:`，按 `--target` 选择 slice；命令行传入的同名 `.a/.xulib` 可临时覆盖 manifest。macOS arm64/x64 已支持一步式系统链接；其它目标先用 `--emit-obj` 配合相应系统 linker。`xu test` 的 native test-harness 链接尚未完成，会明确报错。

底层流程仍可单独执行：`xu build --emit-obj main.xu module.xulib -o main.xuobj` 生成 XUOBJ，再用 `xu _macho_o`、`xu _elf_o` 或 `xu _coff_o` 转换为平台对象；转换后的对象仍需与选中的 native archive slice 一起交给系统 linker。

`xu-c-1` 的 `int` 对应 C `int64_t` 或头文件提供的 `xu_int`。C 导出必须使用 `XU_NATIVE_SYMBOL(...)`，它会处理 Mach-O 与 ELF/COFF 的符号下划线差异。

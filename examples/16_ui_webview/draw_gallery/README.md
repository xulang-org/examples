# 绘图资源图库

需要与本目录 `xu.xun` 的 `xu:` 要求匹配的 SDK，使用 `#ui` / `#ui/draw` 原生宿主，无 WebView。

演示贝塞尔路径、描边、渐变、高阶几何原语（圆角矩形、胶囊、扇形/环形扇区、正多边形、星形、菱形、网格、虚线、箭头、阴影、二次/三次曲线）、色彩工具（`hex` / `hsl` / `lerp_color` / `lighten` / `darken` / `with_opacity`）、内嵌 PNG、采样、透明度、复杂文字排版、离屏图层、`DisplayList` 与 `SurfaceCache`。

## 运行

从本仓库根执行：

```sh
python3 scripts/ui_examples.py run draw_gallery
# 与 xu 不在同级时：
XU_ROOT=/path/to/xu python3 scripts/ui_examples.py run draw_gallery
# 仅检查或打包：
python3 scripts/ui_examples.py check draw_gallery
python3 scripts/ui_examples.py build draw_gallery
```

产物位于 `build/ui/<target>/`；macOS 为 `.app`，Windows 为 `.exe`，Linux 为可执行文件。打包需要对应平台工具链，使用 `xu doctor --desktop --target <target>` 检查。

## 操作

Space 在向量回放和栅格缓存之间切换；R 使缓存失效；窗口 DPI 变化会更新缓存密度；Esc 退出。

## 代码要点

PNG 通过 `www/sample.png` 嵌入，不依赖启动目录。文字、图片、显示列表和缓存均显式释放；已提交的帧仍持有其不可变资源版本。缓存只在源、尺寸、密度变化或失效时重建。

入口见 [main.xu](main.xu)。共享的窗口启动、事件等待和文字辅助函数见 [shared/](../shared/)；应用仍自行持有模型、资源和事件分发，不另开事件消费者。

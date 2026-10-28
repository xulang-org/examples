#!/usr/bin/env python3
"""List, check, package and run the native Xu UI examples with the current compiler."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import platform
import re
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
APPLICATIONS = ROOT / "examples" / "16_ui_webview"
SAMPLES = {
    "component_workshop": "46 native component entries with themes, scale, states and keyboard interaction",
    "draw": "Frame recording, transforms, clipping and pointer input",
    "draw_gallery": "Paths, gradients, images, text, layers and display-list caches",
    "widgets": "Retained layout, controls, scrolling, capture and keyboard focus",
    "text_input": "Native IME, preedit, UTF-8 selection and shaped caret geometry",
    "desktop_services": "Async clipboard, prompt and file dialogs; cancel and timeout",
    "multiwindow": "Dynamic windows, independent canvases and one event channel",
    "workbench": "Image browser combining layout, controls, file requests and preview windows",
    "editor_workbench": "Input, TextArea, structured menus and actual screen queries",
    "component_catalog": "Tables, trees, inputs, anchored overlays, modal focus and feedback",
    "date_time": "Calendar date picker, segmented time picker and typed time values",
    "style_gallery": "Control themes, typography, density, local overrides and styled scrollbars",
    "controls_gallery": "Extended controls, focus, and StyleSwitcher presets, text scale and density",
    "xacp_screen": "XACP Screen tree inspection, screenshots and permissioned UI actions",
    "webview_requests": "Concurrent WebView invokes, cancellation, deadlines and native queries",
    "open": "Existing WebView, menu and tray example",
    "shaped": "Existing transparent shaped window",
    "remote": "Existing remote WebView (requires the local HTTP server)",
}


def native_target() -> str:
    system = {"Darwin": "macos", "Linux": "linux", "Windows": "windows"}.get(platform.system())
    arch = {"arm64": "arm64", "aarch64": "arm64", "x86_64": "x64", "amd64": "x64"}.get(platform.machine().lower())
    if not system or not arch:
        raise ValueError("Pass --target explicitly for this host")
    return f"{system}-{arch}"


def invoke(command: list[str], *, cwd: Path, env: dict[str, str]) -> None:
    print("+ " + shlex.join(command), flush=True)
    subprocess.run(command, cwd=cwd, env=env, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("list", "check", "build", "run"))
    parser.add_argument("sample", nargs="?", default="workbench", choices=[*SAMPLES, "all"])
    parser.add_argument("--target", choices=[f"{system}-{arch}" for system in ("macos", "linux", "windows") for arch in ("arm64", "x64")])
    parser.add_argument("--compiler", type=Path, help="defaults to $XU or $XU_ROOT/bin/xu")
    parser.add_argument("--no-build", action="store_true", help="run an already packaged example")
    args = parser.parse_args()
    if args.action == "list":
        for name, description in SAMPLES.items():
            print(f"{name:18} {description}")
        return 0
    if args.sample == "all" and args.action == "run":
        parser.error("run requires one sample")
    if args.no_build and args.action != "run":
        parser.error("--no-build is only valid with run")
    target = args.target or native_target()
    if args.action == "run" and target != native_target():
        parser.error("run requires the native host target; use build for cross packaging")
    compiler_root = Path(os.environ.get("XU_ROOT", str(ROOT.parent / "xu"))).expanduser().resolve()
    suffix = ".exe" if os.name == "nt" else ""
    compiler = (args.compiler or Path(os.environ.get("XU", str(compiler_root / "bin" / ("xu" + suffix))))).expanduser().resolve()
    if not compiler.is_file():
        parser.error(f"compiler not found: {compiler}; build Xu or set XU_ROOT/XU")
    identity = subprocess.run([str(compiler), "--version"], check=True, text=True, capture_output=True).stdout
    version = re.search(r"(\d+)\.(\d+)\.(\d+)(?:-([0-9A-Za-z.-]+))?", identity)
    if not version or tuple(map(int, version.groups()[:3])) < (0, 10, 1):
        parser.error("these examples require Xu 0.10.1 or newer")
    selected = list(SAMPLES) if args.sample == "all" else [args.sample]
    if "component_workshop" in selected:
        release = tuple(map(int, version.groups()[:3]))
        if release < (0, 10, 10) or (release == (0, 10, 10) and version.group(4)):
            parser.error("component_workshop requires Xu 0.10.10 or newer; older SDKs lack its widget APIs")
    if any(name in ("style_gallery", "date_time", "component_catalog") for name in selected):
        release = tuple(map(int, version.groups()[:3]))
        if release < (0, 10, 1):
            parser.error("style_gallery, date_time and component_catalog require Xu 0.10.1 or newer")
    if "xacp_screen" in selected:
        release = tuple(map(int, version.groups()[:3]))
        dev_iteration = int(version.group(4)) if version.group(4) else None
        if release < (0, 10, 10) or (release == (0, 10, 10) and dev_iteration is not None and dev_iteration < 1):
            parser.error("xacp_screen requires Xu 0.10.10-dev.1 or newer")
    output = ROOT / "build" / "ui" / target
    output.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, XU_ROOT=str(compiler_root), XU_STDLIB_ROOT=str(compiler_root / "src"),
               XU_EXAMPLES_ROOT=str(ROOT), XUPATH="", XU_CACHE=str(ROOT / "build" / "ui" / "package-cache"))
    for key in ("XUOS", "XUARCH", "XU_BOOT_EXTRA", "XU_UI_SIM", "XU_UI_PACK", "XU_UI_TMP"):
        env.pop(key, None)
    # The released compiler selects its larger task stack while emitting UI
    # objects. Set the profile before the desktop packager invokes it.
    if args.action != "check":
        env["XU_UI_PACK"] = "1"
    names = list(SAMPLES) if args.sample == "all" else [args.sample]
    for name in names:
        project = APPLICATIONS / name
        if args.action == "check":
            invoke([str(compiler), "check", str(project), "--target", target], cwd=project, env=env)
            continue
        artifact = output / (name + (".exe" if target.startswith("windows") else ""))
        if not args.no_build:
            pack_options = [] if name in ("open", "remote", "webview_requests", "component_workshop") else ["--no-webview"]
            if (project / "www").is_dir():
                pack_options += ["--www", str(project / "www")]
            invoke([str(compiler), "build", str(project), "--target", target, "--desktop", *pack_options, "-o", str(artifact)], cwd=compiler_root, env=env)
        if target.startswith("macos"):
            artifact = artifact.with_suffix(".app")
        if args.action == "run":
            if not artifact.exists():
                parser.error(f"application not found: {artifact}; omit --no-build")
            command = ["open", "-n", "-W", str(artifact)] if target.startswith("macos") else [str(artifact)]
            invoke(command, cwd=project, env=env)
        else:
            print(f"Packaged {artifact}", flush=True)
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f"ui-examples: {error}", file=sys.stderr)
        raise SystemExit(1)

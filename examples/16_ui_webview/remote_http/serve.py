#!/usr/bin/env python3
"""Standalone static HTTP server for remote WebView IPC demos.

Not part of #ui/webview — Xu does not host this. Serves this directory only.

  python3 examples/16_ui_webview/remote_http/serve.py
  # → http://127.0.0.1:8765/

Then pack/run remote_window (loads that URL and calls xu:// IPC).
"""
from __future__ import annotations

import argparse
import functools
import http.server
import socketserver
from pathlib import Path

HERE = Path(__file__).resolve().parent
DEFAULT_PORT = 8765


class Handler(http.server.SimpleHTTPRequestHandler):
    def log_message(self, fmt: str, *args) -> None:
        print(f"[remote_http] {self.address_string()} {fmt % args}")


def main() -> None:
    ap = argparse.ArgumentParser(description="Serve remote_http/ for Xu load_remote tests")
    ap.add_argument("-p", "--port", type=int, default=DEFAULT_PORT)
    ap.add_argument("--host", default="127.0.0.1")
    args = ap.parse_args()
    handler = functools.partial(Handler, directory=str(HERE))
    socketserver.TCPServer.allow_reuse_address = True
    with socketserver.TCPServer((args.host, args.port), handler) as httpd:
        print(f"serving {HERE}")
        print(f"open http://{args.host}:{args.port}/")
        print("(Ctrl-C to stop; Xu remote_window should load this URL)")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nstopped")


if __name__ == "__main__":
    main()

# Makefile for 20_c_export_lib
.PHONY: all lib win-lib win-test test test-c test-cpp test-rust test-python test-go clean help

SHELL := /bin/bash

XU_DIR := $(abspath $(CURDIR)/../../../xu)
XU := $(if $(wildcard $(XU_DIR)/bin/xu),$(XU_DIR)/bin/xu,xu)
XU_STDLIB_ROOT := $(XU_DIR)/src
export XU_STDLIB_ROOT

all: lib test

lib:
	./build_and_test.sh

win-lib:
	@echo "=== Building Windows Static (xulib.lib) and Dynamic (xulib.dll) Libraries ==="
	XU_STDLIB_ROOT="$(XU_STDLIB_ROOT)" $(XU) build --target windows-x64 --emit-obj main.xu -o libxulib_win.xuobj
	$(XU) _coff_o libxulib_win.xuobj libxulib_win.obj
	@if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then \
		x86_64-w64-mingw32-gcc -c win_bridge.s -o win_bridge.obj; \
		x86_64-w64-mingw32-ar rcs xulib.lib libxulib_win.obj win_bridge.obj; \
		x86_64-w64-mingw32-gcc -shared libxulib_win.obj win_bridge.obj -Wl,--out-implib,libxulib_dll.a -o xulib.dll; \
		echo ">>> Windows libraries successfully built: xulib.lib and xulib.dll"; \
	elif command -v gcc >/dev/null 2>&1 && [ "$$(uname -o 2>/dev/null)" = "Msys" -o "$$(uname -s)" = "MINGW64_NT" ]; then \
		gcc -c win_bridge.s -o win_bridge.obj; \
		ar rcs xulib.lib libxulib_win.obj win_bridge.obj; \
		gcc -shared libxulib_win.obj win_bridge.obj -Wl,--out-implib,libxulib_dll.a -o xulib.dll; \
		echo ">>> Windows libraries successfully built: xulib.lib and xulib.dll"; \
	else \
		echo "[WARN] MinGW x86_64 GCC not found. Generated libxulib_win.obj for Windows."; \
	fi

win-test: win-lib
	@if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then \
		echo "=== Compiling Windows test executables ==="; \
		x86_64-w64-mingw32-gcc test_windows.c -L. -lxulib_dll -o test_windows_dynamic.exe; \
		x86_64-w64-mingw32-gcc test_windows.c xulib.lib -o test_windows_static.exe; \
		echo ">>> Successfully built test_windows_dynamic.exe and test_windows_static.exe"; \
	fi

test: test-c test-cpp test-rust test-python test-go

test-c:
	./test_dynamic_bin
	./test_static_bin

test-cpp:
	./test_cpp_bin

test-rust:
	./test_rust_bin

test-python:
	/usr/bin/python3 test_python.py || /opt/homebrew/bin/python3 test_python.py || python3 test_python.py

test-go:
	./test_go_bin

clean:
	rm -f libxulib.xuobj libxulib.o libxulib.a libxulib.dylib libxulib.so \
	      libxulib_win.xuobj libxulib_win.obj win_bridge.obj libxulib_dll.a \
	      xulib.lib xulib.dll test_windows_dynamic.exe test_windows_static.exe test_csharp.exe \
	      test_dynamic_bin test_static_bin test_cpp_bin test_rust_bin test_go_bin

help:
	@echo "Targets:"
	@echo "  make all         - Build libraries and run all tests (5 host languages)"
	@echo "  make lib         - Build static (.a) and dynamic (.dylib/.so) libraries"
	@echo "  make win-lib     - Build Windows static (xulib.lib) and dynamic (xulib.dll)"
	@echo "  make win-test    - Build Windows test executables (test_windows_dynamic/static.exe)"
	@echo "  make test        - Run tests across all languages"
	@echo "  make test-c      - Run C dynamic and static tests"
	@echo "  make test-cpp    - Run C++ test"
	@echo "  make test-rust   - Run Rust FFI test"
	@echo "  make test-python - Run Python ctypes test"
	@echo "  make test-go     - Run Go cgo test"
	@echo "  make clean       - Remove generated build artifacts"

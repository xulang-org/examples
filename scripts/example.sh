#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd -P)
XU_ROOT=$(CDPATH= cd -- "${XU_ROOT:-$ROOT/../xu}" && pwd -P)
mode=${1:-run}
example=${2:-01_hello}
case "$mode" in run|check|build) ;; *) echo "expected run, check or build" >&2; exit 2 ;; esac
case "$example" in */../*|../*|/*) echo "example must be relative to examples/" >&2; exit 2 ;; esac
source="$ROOT/examples/$example"
[ -d "$source" ] || { echo "missing example: $source" >&2; exit 1; }
if [ ! -f "$source/xu.xun" ]; then source="$source/main.xu"; fi
compiler=$(cd "$XU_ROOT" && XU_ALLOW_ORACLE_SEED=0 XU_SEED="${XU:-}" "$XU_ROOT/scripts/xu_host_seed.sh")
runner="$XU_ROOT/scripts/run_product_compiler.sh"
if [ "$mode" = check ]; then exec "$runner" "$compiler" check "$source"; fi
mkdir -p "$ROOT/build"
artifact="$ROOT/build/$(basename "$example").bin"
"$runner" "$compiler" build "$source" -o "$artifact"
if [ "$mode" = run ]; then cd "$ROOT"; exec "$artifact"; fi

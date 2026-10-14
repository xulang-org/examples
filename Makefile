EXAMPLE ?= 01_hello
.PHONY: run check build
run check build:
	./scripts/example.sh $@ "$(EXAMPLE)"

.PHONY: wasm
wasm:
	$(MAKE) run EXAMPLE=52_wasm

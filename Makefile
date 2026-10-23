EXAMPLE ?= 01_hello
.PHONY: run check build
run check build:
	./scripts/example.sh $@ "$(EXAMPLE)"

.PHONY: wasm
wasm:
	$(MAKE) run EXAMPLE=52_wasm

UI ?= workbench
UI_ARGS ?=
.PHONY: ui-list ui-check ui-build ui-run
ui-list:
	python3 scripts/ui_examples.py list
ui-check:
	python3 scripts/ui_examples.py check "$(UI)" $(UI_ARGS)
ui-build:
	python3 scripts/ui_examples.py build "$(UI)" $(UI_ARGS)
ui-run:
	python3 scripts/ui_examples.py run "$(UI)" $(UI_ARGS)

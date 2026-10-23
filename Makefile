.DEFAULT_GOAL := gba
.NOTPARALLEL:

.PHONY: help gba baseline setup guides pc native

ifeq ($(OS),Windows_NT)
GBA_BUILD = powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/build.ps1
PYTHON ?= python
else
GBA_BUILD = bash scripts/build.sh
PYTHON ?= python3
endif

help:
	@echo "make gba       Build the GBA ROM (default)"
	@echo "make baseline  Build and verify the original GBA reference"
	@echo "make pc        Build the PC target"
	@echo "make native    Alias for the PC target"
	@echo "make setup     Check bundled sources"
	@echo "make guides    Regenerate documentation from game data"

gba:
	$(GBA_BUILD) hack

baseline:
	$(GBA_BUILD) baseline

setup:
	$(PYTHON) scripts/source_base.py --check-patch

guides:
	$(PYTHON) scripts/generate_guides.py

pc native:
	@echo "PC backend selection is being configured."
	@exit 1

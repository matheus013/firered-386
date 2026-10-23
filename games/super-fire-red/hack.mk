# Hack build entry point. From the repository root: make -C games/super-fire-red -f hack.mk <target>
# (games/super-fire-red/Makefile is the pokefirered decomp's own Makefile.)
.DEFAULT_GOAL := gba
.NOTPARALLEL:

.PHONY: help gba baseline setup setup-native guides pc native

ifeq ($(OS),Windows_NT)
GBA_BUILD = powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/build.ps1
PYTHON ?= python
PC_PLATFORM ?= windows
PC_BUILD = powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/build_native.ps1 -Platform $(PC_PLATFORM)
NATIVE_SETUP = wsl -d Ubuntu -u root -- bash scripts/setup_native.sh
else
GBA_BUILD = bash scripts/build.sh
PYTHON ?= python3
PC_PLATFORM ?= $(if $(WSL_DISTRO_NAME),windows,linux)
PC_BUILD = $(PYTHON) scripts/build_native.py --platform $(PC_PLATFORM)
NATIVE_SETUP = bash scripts/setup_native.sh
endif

help:
	@echo "make -C games/super-fire-red -f hack.mk gba       Build the GBA ROM (default)"
	@echo "make -C games/super-fire-red -f hack.mk baseline  Build and verify the original GBA reference"
	@echo "make -C games/super-fire-red -f hack.mk pc        Build native PC game (PC_PLATFORM=windows or linux)"
	@echo "make -C games/super-fire-red -f hack.mk setup-native Install native build dependencies on Ubuntu/WSL"
	@echo "make -C games/super-fire-red -f hack.mk native    Alias for the PC target"
	@echo "make -C games/super-fire-red -f hack.mk setup     Check bundled sources"
	@echo "make -C games/super-fire-red -f hack.mk guides    Regenerate documentation from game data"

gba:
	$(GBA_BUILD) hack

baseline:
	$(GBA_BUILD) baseline

setup:
	$(PYTHON) scripts/source_base.py --check-patch

guides:
	$(PYTHON) scripts/generate_guides.py

setup-native:
	$(NATIVE_SETUP)

pc native: gba
	$(PC_BUILD)

# ==============================================================================
# ToneProject - Master Makefile
# ==============================================================================

.DEFAULT_GOAL := help
SHELL         := /bin/bash
PYTHON        := python3
PIO           := pio

# Colors for terminal output
COLOR_RESET   := \033[0m
COLOR_CYAN    := \033[36m
COLOR_GREEN   := \033[32m
COLOR_YELLOW  := \033[33m

.PHONY: help
help: ## Show this help message and target descriptions
	@echo ""
	@echo -e "$(COLOR_CYAN)ToneProject Build & Automation Commands$(COLOR_RESET)"
	@echo -e "$(COLOR_YELLOW)Usage: make [target]$(COLOR_RESET)"
	@echo ""
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  $(COLOR_GREEN)%-22s$(COLOR_RESET) %s\n", $$1, $$2}'
	@echo ""

# ------------------------------------------------------------------------------
# Firmware Targets (ToneOS - ESP32 PlatformIO)
# ------------------------------------------------------------------------------

.PHONY: firmware-build
firmware-build: ## Compile ToneOS firmware for default target (ESP32 DevKit WROOM)
	@echo -e "$(COLOR_CYAN)Compiling ToneOS for ESP32 DevKit...$(COLOR_RESET)"
	$(PIO) run -d libs/ToneOS -e esp32dev

.PHONY: firmware-build-c3
firmware-build-c3: ## Compile ToneOS firmware for ESP32-C3 (RISC-V) target
	@echo -e "$(COLOR_CYAN)Compiling ToneOS for ESP32-C3...$(COLOR_RESET)"
	$(PIO) run -d libs/ToneOS -e esp32-c3-devkitm-1

.PHONY: firmware-upload
firmware-upload: ## Flash ToneOS firmware to connected ESP32 board
	@echo -e "$(COLOR_CYAN)Uploading ToneOS to target board...$(COLOR_RESET)"
	$(PIO) run -d libs/ToneOS --target upload

.PHONY: firmware-upload-c3
firmware-upload-c3: ## Flash ToneOS firmware to connected ESP32-C3 board
	@echo -e "$(COLOR_CYAN)Uploading ToneOS to ESP32-C3 board...$(COLOR_RESET)"
	$(PIO) run -d libs/ToneOS -e esp32-c3-devkitm-1 --target upload

.PHONY: firmware-monitor
firmware-monitor: ## Open serial device monitor (115200 baud)
	@echo -e "$(COLOR_CYAN)Connecting to serial monitor at 115200 baud...$(COLOR_RESET)"
	$(PIO) device monitor -d libs/ToneOS -b 115200

.PHONY: firmware-clean
firmware-clean: ## Clean PlatformIO build cache and temporary objects
	@echo -e "$(COLOR_CYAN)Cleaning PlatformIO build directory...$(COLOR_RESET)"
	$(PIO) run -d libs/ToneOS --target clean
	rm -rf libs/ToneOS/.pio

# ------------------------------------------------------------------------------
# Desktop Client Targets (ToneTerminal - Python)
# ------------------------------------------------------------------------------

.PHONY: terminal-install
terminal-install: ## Install ToneTerminal Python dependencies
	@echo -e "$(COLOR_CYAN)Installing ToneTerminal dependencies...$(COLOR_RESET)"
	$(PYTHON) -m pip install -r libs/ToneTerminal/requirements.txt

.PHONY: terminal-run
terminal-run: ## Start ToneTerminal companion background service
	@echo -e "$(COLOR_CYAN)Launching ToneTerminal...$(COLOR_RESET)"
	$(PYTHON) libs/ToneTerminal/main.py

.PHONY: test
test: ## Run ToneTerminal unit test suite
	@echo -e "$(COLOR_CYAN)Running ToneTerminal test suite...$(COLOR_RESET)"
	$(PYTHON) -m unittest discover libs/ToneTerminal/tests

# ------------------------------------------------------------------------------
# Documentation & Local Web Preview
# ------------------------------------------------------------------------------

.PHONY: docs-serve
docs-serve: ## Launch local HTTP server to preview Tone documentation site (http://localhost:8000)
	@echo -e "$(COLOR_CYAN)Serving documentation at http://localhost:8000/ ...$(COLOR_RESET)"
	$(PYTHON) -m http.server 8000 --directory docs

# ------------------------------------------------------------------------------
# Meta & Cleanup
# ------------------------------------------------------------------------------

.PHONY: test-all
test-all: test firmware-build ## Run both Python unit tests and firmware compilation checks
	@echo -e "$(COLOR_GREEN)All tests and firmware compilation checks passed successfully!$(COLOR_RESET)"

.PHONY: clean
clean: firmware-clean ## Clean all local Python bytecode, test cache, and firmware artifacts
	@echo -e "$(COLOR_CYAN)Cleaning Python cache directories...$(COLOR_RESET)"
	find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
	find . -type f -name "*.pyc" -delete 2>/dev/null || true
	rm -rf .pytest_cache
	@echo -e "$(COLOR_GREEN)Repository cleaned.$(COLOR_RESET)"

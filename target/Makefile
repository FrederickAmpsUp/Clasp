PROJECT_ROOT = ../
TARGET_SOURCE = ./printer.c
SRC_DIR := $(PROJECT_ROOT)/src/
INC_DIR := $(PROJECT_ROOT)/include/
BIN_DIR := $(PROJECT_ROOT)/bin/

TARGET_MODE = visitor
OUTPUT := ./$(notdir $(basename $(TARGET_SOURCE))).cbt

PACKAGE_SCRIPT := python3 $(PROJECT_ROOT)/util/make_target_cl.py

default: compile package

compile:
	@gcc -shared -o $(BIN_DIR)/target_$(notdir $(basename $(TARGET_SOURCE))).so -I $(INC_DIR) $(shell find $(SRC_DIR)/** -name '*.c') -fPIC $(TARGET_SOURCE)
package:
	@$(PACKAGE_SCRIPT) $(OUTPUT) $(TARGET_MODE) $(BIN_DIR)/target_$(notdir $(basename $(TARGET_SOURCE))).so
# Same as ATM

CXX=g++
COMPILER_FLAGS=-Wall -Wextra -MMD -MP
INCLUDE_PATH=include utils include/backend
OUTPUT_DIR=build
SOURCE_DIR=src
PROGRAM=bank

EXTRA_FLAGS=${addprefix -I, ${INCLUDE_PATH}}
SOURCE_FILES=$(foreach path, $(SOURCE_DIR), $(wildcard $(path)/*.cpp))
FINAL_OUTPUT_FILE_NAMES=$(patsubst $(SOURCE_DIR)/%.cpp, $(OUTPUT_DIR)/%.o, $(SOURCE_FILES))
DEPENDENCIES=$(patsubst $(OUTPUT_DIR)/%.o, $(OUTPUT_DIR)/%.d, $(FINAL_OUTPUT_FILE_NAMES))

all: preinit ${PROGRAM}

preinit: create_dirs
ifeq ($(mode), debug)
		@echo "Building in DEBUG mode..."
		COMPILER_FLAGS+=-g
else
		@echo "Building in RELEASE mode..."
		COMPILER_FLAGS+=-O3
endif

create_dirs:
	@mkdir -p $(OUTPUT_DIR)
	$(foreach path, $(SOURCE_DIR), $(shell mkdir -p $(OUTPUT_DIR)/$(path)))

$(PROGRAM): preinit $(FINAL_OUTPUT_FILE_NAMES)
	$(CXX) $(COMPILER_FLAGS) ${EXTRA_FLAGS} main.cpp -o $(OUTPUT_DIR)/$(PROGRAM) $(FINAL_OUTPUT_FILE_NAMES)

$(OUTPUT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(COMPILER_FLAGS) ${EXTRA_FLAGS} -c $< -o $@

clean:
	@rm -rf $(OUTPUT_DIR)
	@echo "Build artifacts cleaned."

.PHONY: preinit create_dirs clean all

-include $(DEPENDENCIES)

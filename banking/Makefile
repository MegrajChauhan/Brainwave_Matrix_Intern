# Same as ATM

CXX=g++
COMPILER_FLAGS=-Wall -Wextra -std=c++23 -MMD -MP
INCLUDE_PATH=include utils include/backend
OUTPUT_DIR=build
SOURCE_DIR=src
PROGRAM=bank

EXTRA_FLAGS=$(addprefix -I, $(INCLUDE_PATH))
SOURCE_FILES=$(foreach path, $(SOURCE_DIR), $(wildcard $(path)/*.cpp))
FINAL_OUTPUT_FILE_NAMES=$(patsubst $(SOURCE_DIR)/%.cpp, $(OUTPUT_DIR)/%.o, $(SOURCE_FILES))
DEPENDENCIES=$(patsubst $(OUTPUT_DIR)/%.o, $(OUTPUT_DIR)/%.d, $(FINAL_OUTPUT_FILE_NAMES))
LINK_FLAGS=

ifeq ($(OS), Windows_NT)
    MKDIR=mkdir
    RM=del /Q
    LINK_FLAGS=-lws2_32
else
    MKDIR=mkdir -p
    RM=rm -rf
endif

all: preinit $(PROGRAM)

preinit: create_dirs
ifeq ($(mode), debug)
	@echo "Building in DEBUG mode..."
	COMPILER_FLAGS+=-g
else
	@echo "Building in RELEASE mode..."
	COMPILER_FLAGS+=-O3
endif

create_dirs:
	$(MKDIR) $(OUTPUT_DIR)
	$(foreach path, $(SOURCE_DIR), $(MKDIR) $(OUTPUT_DIR)/$(path))

$(PROGRAM): preinit $(FINAL_OUTPUT_FILE_NAMES)
	$(CXX) $(COMPILER_FLAGS) ${EXTRA_FLAGS} main.cpp -o $(OUTPUT_DIR)/$(PROGRAM) $(FINAL_OUTPUT_FILE_NAMES)
	$(CXX) $(COMPILER_FLAGS) ${EXTRA_FLAGS} server.cpp -o $(OUTPUT_DIR)/server $(FINAL_OUTPUT_FILE_NAMES)

$(OUTPUT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(COMPILER_FLAGS) ${EXTRA_FLAGS} -c $< -o $@

clean:
	$(RM) $(OUTPUT_DIR)
	@echo "Build artifacts cleaned."

# Include dependencies
-include $(DEPENDENCIES)

.PHONY: preinit create_dirs clean all

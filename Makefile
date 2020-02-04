# Makefile to cover general C project needs, including the use of different
# compilers, debugging, sanitizing, creating tags for editors, and profiling.
#
# Targets:
#   all     -- Compile project executable.
#   profile -- Same as 'all' except with profiling enabled.
#   tests   -- Compile all test files.
#   paths   -- Ensures the build path is structured properly.
#   clean   -- Remove object files and project executables from build path.
#   tags    -- Create tags for Universal Ctags.
#
# Configurable variables:
#   CC              -- The C compiler used. This Makefile is designed for gcc or clang.
#   DEBUG           -- Define to enable various debug features, such as sanitizers.
#   PROJECT_ROOT    -- The base directory of the project.
#   SRC_DIR         -- Directory to search for C source files (*.c).
#   INCLUDE_DIR     -- Directory to search for C header files (*.h).
#   BUILD_DIR       -- Directory to use for compiler output (*.o and executables).
#
# Other variables:
#   LIBRARIES   -- Library flags to pass to the compiler.
#   OBJECTS     -- Object file names to compile for project.

#################
# project files #
#################

PROJECT_ROOT ?= .

SRC_DIR ?= $(PROJECT_ROOT)/src
INCLUDE_DIR ?= $(PROJECT_ROOT)/include
BUILD_DIR ?= $(PROJECT_ROOT)/build

######################
# C compiler options #
######################

LIBRARIES := 
OBJECTS := 

OBJECT_FILES := $(foreach object, $(OBJECTS), $(BUILD_DIR)/$(object))

CC ?= gcc
CFLAGS := -I $(INCLUDE_DIR) -std=c99 -Wall -Wextra -Werror -pedantic

ifdef DEBUG
# default debug flags:
#   -O0                 -- disables optimizations to allow seamless debugging
#   -g3                 -- generates all debug symbols (including for macros)
#   -fsanitize=[...]    -- enables sanitizers at runtime to check code behavior
CFLAGS += -O0 -g3 -fsanitize=address,leak,undefined

ifneq (,$(filter $(CC),cc gcc))
# if the C compiler is gcc (under the assumption that cc points to gcc),
# statically link libasan to avoid potential issues with library link order.
# libasan is required for -fsanitize=address with gcc
LIBRARIES += -static-libasan
endif

else
# default non-debug flags:
#   -O3         -- allows high code optimization
#   -DNDEBUG    -- signals that we aren't in debug mode for the C program
CFLAGS += -O3 -DNDEBUG
endif

#################
# build targets #
#################

.PHONY: all profile tests paths clean tags

all: paths clean $(OBJECTS)
	$(CC) $(LIBRARIES) $(CFLAGS) $(OBJECT_FILES) $(SRC_DIR)/main.c -o $(BUILD_DIR)/main

# default rule for an object file -- assumes no
# other source files or libraries are needed
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(SRC_DIR)/$*.c -c -o $(BUILD_DIR)/$*.o

profile: CFLAGS += -pg
profile: all

tests: CFLAGS += -UNDEBUG -Wno-error
tests: paths test-trie test-outstream

# default rule for test file -- no object dependencies
# and outputs executable in $(BUILD_DIR)/tests
test-%: tests/test_%.c
	$(CC) $(LIBRARIES) $(CFLAGS) $(SRC_DIR)/$*.c $^ -o $(BUILD_DIR)/tests/$@

paths:
	mkdir -p $(BUILD_DIR)/tests

clean:
	find $(BUILD_DIR) \
		\( -name "*.o" -o -executable -a \! -type d \) \
		-exec rm -f {} \;

tags:
	ctags -R

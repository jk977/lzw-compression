ROOT ?= .

SRC ?= $(ROOT)/src
INCLUDE ?= $(ROOT)/include
BUILD ?= $(ROOT)/build

OBJECTS := instream.o outstream.o sequence.o trie.o lzwcontext.o lzw.o
OBJECT_FILES := $(foreach object, $(OBJECTS), $(BUILD)/$(object))

CC ?= gcc
CFLAGS := -I $(INCLUDE) -std=c99 -Wall -Wextra -Werror -pedantic

ifdef DEBUG
CFLAGS += -O0 -g3
else
CFLAGS += -O2 -DNDEBUG
endif

#################
# build targets #
#################

.PHONY: all lib tests paths clean

cli: CFLAGS += -D_POSIX_C_SOURCE=2
cli: lib
	$(CC) -L./build $(CFLAGS) $(OBJECT_FILES) $(SRC)/main.c -llzw -o $(BUILD)/lzw

lib: paths clean $(OBJECT_FILES)
	ar rcs $(BUILD)/liblzw.a $(OBJECT_FILES)

$(BUILD)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(SRC)/$*.c -c -o $@

tests: CFLAGS += -UNDEBUG -Wno-error
tests: paths test-trie test-outstream test-instream test-lzw

test-lzw: tests/test_lzw.c
	$(CC) $(CFLAGS) $(SRC)/*.c $^ -o $(BUILD)/tests/$@

test-%: tests/test_%.c
	$(CC) $(CFLAGS) $(SRC)/$*.c $^ -o $(BUILD)/tests/$@

paths:
	mkdir -p $(BUILD)/tests

clean:
	find $(BUILD) \
		\( -name "*.o" -o -executable -a \! -type d \) \
		-exec rm -f {} \;

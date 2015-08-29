
all: test

#
# VARS
#

CC=clang
CFLAGS=-Wall -Werror -g -O0

BUILD_DIR=build
BUILD_DIR_CREATED=$(BUILD_DIR)/tuna

SRC_DIR=src
INC_DIR=inc
TST_DIR=tst

VPATH=$(SRC_DIR) $(INC_DIR) $(TST_DIR)

COMMON_FILES=$(SRC_DIR)/heap.c $(SRC_DIR)/util.c

#
# BUILD
#

$(BUILD_DIR_CREATED):
	mkdir $(@D)
	touch $@

$(BUILD_DIR)/%.o: %.c heap.h | $(BUILD_DIR_CREATED)
	$(CC) $(CFLAGS) -I. -I$(INC_DIR) -o $@ -c $<

clean:
	rm -rf $(BUILD_DIR) $(shell find . -name \*~)

#
# TEST
#

TESTS=       \
    heap     \
    matrix   \
    util     \

test: $(patsubst %, run-%-test, $(TESTS))

HEAP_TST_SRC= $(SRC_DIR)/heap.c $(TST_DIR)/heap-test.c
$(BUILD_DIR)/heap-test: $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(HEAP_TST_SRC)))
	$(CC) -lmcgoo -o $@ $^
run-heap-test: $(BUILD_DIR)/heap-test
	./$<

MATRIX_TST_SRC=$(SRC_DIR)/matrix.c $(COMMON_FILES) $(TST_DIR)/matrix-test.c
$(BUILD_DIR)/matrix-test: $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(MATRIX_TST_SRC)))
	$(CC) -lmcgoo -o $@ $^
run-matrix-test: $(BUILD_DIR)/matrix-test
	./$<

UTIL_TST_SRC=$(SRC_DIR)/util.c $(TST_DIR)/util-test.c
$(BUILD_DIR)/util-test: $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(UTIL_TST_SRC)))
	$(CC) -lmcgoo -o $@ $^
run-util-test: $(BUILD_DIR)/util-test
	./$<

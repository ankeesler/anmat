
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

#
# BUILD
#

$(BUILD_DIR_CREATED):
	mkdir $(@D)
	touch $@

$(BUILD_DIR)/%.o: %.c heap.h | $(BUILD_DIR_CREATED)
	$(CC) $(CFLAGS) -I. -I$(INC_DIR) -o $@ -c $<

clean:
	rm -rf $(BUILD_DIR)

#
# TEST
#

TESTS=       \
    heap     \
    matrix   \

test: $(patsubst %, run-%-test, $(TESTS))

$(BUILD_DIR)/heap-test: $(BUILD_DIR)/heap.o $(BUILD_DIR)/heap-test.o
	$(CC) -lmcgoo -o $@ $^

run-heap-test: $(BUILD_DIR)/heap-test
	./$<

$(BUILD_DIR)/matrix-test: $(BUILD_DIR)/heap.o $(BUILD_DIR)/matrix.o $(BUILD_DIR)/matrix-test.o
	$(CC) -lmcgoo -o $@ $^

run-matrix-test: $(BUILD_DIR)/matrix-test
	./$<

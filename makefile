
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
TEST_DIR=test

VPATH=$(SRC_DIR) $(INC_DIR) $(TEST_DIR)

#
# BUILD
#

$(BUILD_DIR_CREATED):
	mkdir $(@D)
	touch $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR_CREATED)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ -c $<

clean:
	rm -rf $(BUILD_DIR)

#
# TEST
#

TESTS=       \
    matrix   \

test: $(patsubst %, run-%-test, $(TESTS))

$(BUILD_DIR)/matrix-test: $(BUILD_DIR)/matrix.o $(BUILD_DIR)/matrix-test.o
	$(CC) -lmcgoo -o $@ $^

run-matrix-test: $(BUILD_DIR)/matrix-test
	./$<

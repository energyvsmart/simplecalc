CC := gcc

LIBS :=-lm

CFLAGS := -c -g -O2 -Wall -pedantic

SRC_DIR := src

BUILD := .build

BIN_DIR := bin

OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(wildcard $(SRC_DIR)/*.c))

TARGET = $(BIN_DIR)/sm_calc

TEST = python3 test.py 

.PHONY: all test

all: $(TARGET)

test: $(TARGET)
	$(TEST)

$(TARGET): $(BUILD) $(BIN_DIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)


$(BUILD)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD):
	@if test -d $@ ; then true; else mkdir $@; fi
	
$(BIN_DIR):
	@if test -d $@ ; then true; else mkdir $@; fi

clean:
	$(RM) $(TARGET)
	$(RM) -r $(BIN_DIR)
	$(RM) -r $(BUILD)





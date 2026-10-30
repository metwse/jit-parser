MODE ?= release
PLAT ?= armthumb


DIST_DIR = dist
TARGET_DIR = $(DIST_DIR)/$(MODE)
OBJ_DIR = $(TARGET_DIR)/obj

SRC_DIR = src
TEST_DIR = tests
PLAT_DIR = plat/$(PLAT)


CPPFLAGS := $(CPPFLAGS) -MMD -MD

CFLAGS_COMMON = -std=c99 -Wall -Wextra -pedantic -fPIC
CFLAGS_release = -O3 -flto
CFLAGS_debug = -O0 -g3
CFLAGS_test = -O0 -g3 --coverage
CFLAGS_PLAT_armthumb = -mthumb

CFLAGS2 = $(CFLAGS_$(MODE))
ifeq ($(CFLAGS2),)
$(error "Unknown mode $(MODE)")
endif

ifeq ($(wildcard $(PLAT_DIR)),)
$(error "Unknown platform $(PLAT)")
endif

CFLAGS := $(CFLAGS) $(CFLAGS2) $(CFLAGS_PLAT_$(PLAT))


SRCS = $(wildcard $(SRC_DIR)/*.c)
PLAT_SRCS = $(wildcard $(PLAT_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
PLAT_OBJS = $(patsubst $(PLAT_DIR)/%.c,$(OBJ_DIR)/%.$(PLAT).o,$(PLAT_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.test.o,$(TEST_SRCS))


TARGET = $(TARGET_DIR)/jit-parser.a
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c,$(TARGET_DIR)/%.test,$(TEST_SRCS))

default: all

all: $(TARGET) $(TEST_TARGETS)


$(TARGET): $(OBJS) $(PLAT_OBJS) | $(TARGET_DIR)/
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ $< -c $(CPPFLAGS)

$(OBJ_DIR)/%.$(PLAT).o: $(PLAT_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ $< -c $(CPPFLAGS)

$(TARGET_DIR)/%.test: $(OBJ_DIR)/%.test.o $(OBJS) $(PLAT_OBJS) | $(TARGET_DIR)/
	$(CC) $(CFLAGS) -o $@ $^ $(CPPFLAGS)

.SECONDARY:
$(OBJ_DIR)/%.test.o: $(TEST_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ $< -c $(CPPFLAGS)


$(DIST_DIR)%/:
	mkdir -p $@


clean:
	$(RM) -r $(DIST_DIR)

.PHONY: clean default all

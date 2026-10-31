MODE ?= release
PLAT ?= armthumb
OS ?= linux


DIST_DIR = dist
TARGET_DIR = $(DIST_DIR)/$(MODE)
OBJ_DIR = $(TARGET_DIR)/obj

SRC_DIR = src
TEST_DIR = tests
PLAT_DIR = plat/$(PLAT)
OSAL_DIR = osal
VENDOR_DIR = vendor


CPPFLAGS := $(CPPFLAGS) -MMD -MD

CFLAGS_COMMON = -std=c11 -Wall -Wextra -pedantic -fPIC -DLIBFUN_ASSERTIONS
CFLAGS_release = -O3 -flto
CFLAGS_debug = -O0 -g3
CFLAGS_test = -O0 -g3 --coverage
CFLAGS_PLAT_armthumb = -mthumb -DARMTHUMB

CFLAGS2 = $(CFLAGS_$(MODE))
ifeq ($(CFLAGS2),)
$(error "Unknown mode $(MODE)")
endif

ifeq ($(wildcard $(PLAT_DIR)),)
$(error "Unknown platform $(PLAT)")
endif

ifeq ($(wildcard $(OSAL_DIR)/$(OS).c),)
$(error "Unknown OS $(OS)")
endif

CFLAGS := $(CFLAGS) $(CFLAGS_COMMON) $(CFLAGS2) $(CFLAGS_PLAT_$(PLAT))


SRCS = $(wildcard $(SRC_DIR)/*.c)
PLAT_SRCS = $(wildcard $(PLAT_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
OSAL_SRCS = $(wildcard $(OSAL_DIR)/$(OS)*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
PLAT_OBJS = $(patsubst $(PLAT_DIR)/%.c,$(OBJ_DIR)/%.$(PLAT).o,$(PLAT_SRCS))
OSAL_OBJS = $(patsubst $(OSAL_DIR)/%.c,$(OBJ_DIR)/%.$(OS).o,$(OSAL_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.test.o,$(TEST_SRCS))


TARGET = $(TARGET_DIR)/jit-parser.a
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c,$(TARGET_DIR)/%.test,$(TEST_SRCS))

default: all

# External libraries
LIBFUN_DIR := $(VENDOR_DIR)/libfun
LIBFUN_MODE := release

$(LIBFUN_DIR)/libfun.mk: | $(LIBFUN_DIR)/
	cd $(LIBFUN_DIR)/ && \
		git init -q && \
		git remote add origin https://github.com/metwse/libfun.git && \
		git fetch --depth 1 origin 4c6941f60e9b596a5615152eab48e0e0c958b5ec && \
		git checkout -q FETCH_HEAD

include $(LIBFUN_DIR)/libfun.mk

all: $(TARGET) $(TEST_TARGETS)


$(TARGET): $(OBJS) $(PLAT_OBJS) $(OSAL_OBJS) $(LIBFUN) | $(TARGET_DIR)/
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ $< -c $(CPPFLAGS)

$(OBJ_DIR)/%.$(PLAT).o: $(PLAT_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ $< -c $(CPPFLAGS)

$(OBJ_DIR)/%.$(OS).o: $(OSAL_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ $< -c $(CPPFLAGS)

$(TARGET_DIR)/%.test: $(OBJ_DIR)/%.test.o $(OBJS) $(PLAT_OBJS) $(OSAL_OBJS) \
		$(LIBFUN) | $(TARGET_DIR)/
	$(CC) $(CFLAGS) -o $@ $^ $(CPPFLAGS)

.SECONDARY:
$(OBJ_DIR)/%.test.o: $(TEST_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ $< -c $(CPPFLAGS)


$(DIST_DIR)%/ $(VENDOR_DIR)%/:
	mkdir -p $@


clean:
	$(RM) -r $(DIST_DIR)

.PHONY: clean default all

include $(wildcard $(OBJ_DIR)/*.d)

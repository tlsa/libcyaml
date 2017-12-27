# SPDX-License-Identifier: ISC
#
# Copyright (C) 2017 Michael Drake <tlsa@netsurf-browser.org>

VARIANT = debug
VALID_VARIANTS := release debug

ifneq ($(filter $(VARIANT),$(VALID_VARIANTS)),)
else
	$(error VARIANT must be 'release' (default) or 'debug')
endif

.IMPLICIT =

CC = gcc
AR = $(CC)
LD = $(CC)
MKDIR =	mkdir -p

INCLUDE = -I include -I src
CCFLAGS += $(INCLUDE)
CCFLAGS += -std=c11 -Wall -Wextra -pedantic
LDFLAGS = -lyaml
ARFLAGS = -shared

ifeq ($(VARIANT), debug)
	CCFLAGS += -O0 -g
else
	CCFLAGS += -O2 -DNDEBUG
endif

ifneq ($(filter coverage,$(MAKECMDGOALS)),)
	BUILDDIR = build/coverage/$(VARIANT)
	CCFLAGS += -O0
	CCFLAGS_COV = --coverage
	LDFLAGS_COV = --coverage
else
	BUILDDIR = build/$(VARIANT)
	CCFLAGS_COV =
	LDFLAGS_COV =
endif

LIB_SRC_FILES = free.c load.c util.c
LIB_SRC := $(addprefix src/,$(LIB_SRC_FILES))
LIB_OBJ = $(patsubst %.c,%.o, $(addprefix $(BUILDDIR)/,$(LIB_SRC)))

TEST_SRC_FILES = units/free.c units/load.c units/test.c
TEST_SRC := $(addprefix test/,$(TEST_SRC_FILES))
TEST_OBJ = $(patsubst %.c,%.o, $(addprefix $(BUILDDIR)/,$(TEST_SRC)))

TEST_BINS = $(BUILDDIR)/test/units/libcyaml

all: $(BUILDDIR)/libcyaml.so

test: $(TEST_BINS)
	@for i in $(^); do $$i || exit; done

test-quiet: $(TEST_BINS)
	@for i in $(^); do $$i -q || exit; done

test-verbose: $(TEST_BINS)
	@for i in $(^); do $$i -v || exit; done

test-debug: $(TEST_BINS)
	@for i in $(^); do $$i -d || exit; done

$(BUILDDIR)/libcyaml.so: $(LIB_OBJ)
	$(AR) $(ARFLAGS) $(LDFLAGS_COV) -o $@ $^

$(LIB_OBJ): $(BUILDDIR)/%.o : %.c
	$(MKDIR) $(BUILDDIR)/src
	$(CC) $(CCFLAGS) -fPIC $(CCFLAGS_COV) -c -o $@ $<

clean:
	rm -rf build/

.PHONY: all test test-quiet test-verbose test-debug \
		clean

TEST_DEPS = $(BUILDDIR)/libcyaml.so

$(BUILDDIR)/test/units/libcyaml: $(TEST_OBJ) $(TEST_DEPS)
	$(LD) $(LDFLAGS_COV) -o $@ $^ $(LDFLAGS)

$(TEST_OBJ): $(BUILDDIR)/%.o : %.c
	@$(MKDIR) $(BUILDDIR)/test/units
	$(CC) $(CCFLAGS) $(CCFLAGS_COV) -c -o $@ $<

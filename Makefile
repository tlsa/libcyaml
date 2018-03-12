# SPDX-License-Identifier: ISC
#
# Copyright (C) 2017-2018 Michael Drake <tlsa@netsurf-browser.org>

VARIANT = debug
VALID_VARIANTS := release debug

ifneq ($(filter $(VARIANT),$(VALID_VARIANTS)),)
else
$(error VARIANT must be 'debug' (default) or 'release')
endif

.IMPLICIT =

CC = gcc
AR = $(CC)
LD = $(CC)
MKDIR =	mkdir -p
VALGRIND = valgrind --leak-check=full --track-origins=yes

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

coverage: test-verbose
	@$(MKDIR) $(BUILDDIR)
	@gcovr -e 'test/.*' -r .
	@gcovr -e 'test/.*' -x -o build/coverage.xml -r .
	@gcovr -e 'test/.*' --html --html-details -o build/coverage.html -r .

test: $(TEST_BINS)
	@for i in $(^); do $$i || exit; done

test-quiet: $(TEST_BINS)
	@for i in $(^); do $$i -q || exit; done

test-verbose: $(TEST_BINS)
	@for i in $(^); do $$i -v || exit; done

test-debug: $(TEST_BINS)
	@for i in $(^); do $$i -d || exit; done

valgrind: $(TEST_BINS)
	@for i in $(^); do $(VALGRIND) $$i || exit; done

valgrind-quiet: $(TEST_BINS)
	@for i in $(^); do $(VALGRIND) $$i -q || exit; done

valgrind-verbose: $(TEST_BINS)
	@for i in $(^); do $(VALGRIND) $$i -v || exit; done

valgrind-debug: $(TEST_BINS)
	@for i in $(^); do $(VALGRIND) $$i -d || exit; done

$(BUILDDIR)/libcyaml.so: $(LIB_OBJ)
	$(AR) $(ARFLAGS) $(LDFLAGS_COV) -o $@ $^

$(LIB_OBJ): $(BUILDDIR)/%.o : %.c
	$(MKDIR) $(BUILDDIR)/src
	$(CC) $(CCFLAGS) -fPIC $(CCFLAGS_COV) -c -o $@ $<

docs:
	$(MKDIR) build/docs/api
	$(MKDIR) build/docs/devel
	doxygen docs/api.doxygen.conf
	doxygen docs/devel.doxygen.conf

clean:
	rm -rf build/

.PHONY: all test test-quiet test-verbose test-debug \
		valgrind valgrind-quiet valgrind-verbose valgrind-debug \
		clean coverage docs

TEST_DEPS = $(BUILDDIR)/libcyaml.so

$(BUILDDIR)/test/units/libcyaml: $(TEST_OBJ) $(TEST_DEPS)
	$(LD) $(LDFLAGS_COV) -o $@ $^ $(LDFLAGS)

$(TEST_OBJ): $(BUILDDIR)/%.o : %.c
	@$(MKDIR) $(BUILDDIR)/test/units
	$(CC) $(CCFLAGS) $(CCFLAGS_COV) -c -o $@ $<

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

LIB_SRC_FILES = util.c
LIB_SRC := $(addprefix src/,$(LIB_SRC_FILES))
LIB_OBJ = $(patsubst %.c,%.o, $(addprefix $(BUILDDIR)/,$(LIB_SRC)))

all: $(BUILDDIR)/libcyaml.so

$(BUILDDIR)/libcyaml.so: $(LIB_OBJ)
	$(AR) $(ARFLAGS) $(LDFLAGS_COV) -o $@ $^

$(LIB_OBJ): $(BUILDDIR)/%.o : %.c
	$(MKDIR) $(BUILDDIR)/src
	$(CC) $(CCFLAGS) -fPIC $(CCFLAGS_COV) -c -o $@ $<

clean:
	rm -rf build/

.PHONY: all \
		clean

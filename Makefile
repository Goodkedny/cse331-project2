################################################################################
# Makefile
# Izzy Cecil, 2014
#
# `make all` will build the simulator.
# `make test` will build and run unit tests.
#
# All .cc files in the ./src directory are automatically tracked and managed, so
# that if a new file is added, the build does not need to be changed. This file
# will also automatically track interfile dependencies.
#
# All .cc files in the ./test/unit directory are tracked in the same way, and
# will be linked together by the `test` rule, which makes and runs the unit
# test suit.
#
# FLAG env variable will be added to CPPFLAGS, if you need to
# pass compile options from make.
#
# Variables of note:
# LIB <- What library flags need to be given for linking.
# INC <- What directories need to be included for header files?
################################################################################

# Compilers
CC  = clang --std=c11
CPP = clang++

# Compiler Flags
CFLAGS   = -g -Wall -Wextra
CFLAGS  += $(FLAG)
LIB       := -lm
INC       := -I include

# Directories
SRCDIR   := src
BUILDDIR := build
TESTDIR  := test
TARGET   := bin/simulator
TEST     := $(TARGET)_test

# Auto-find source files and track them
CSRCEXT  := c
CPPSRCEXT:= cc
SOURCES  := $(shell find $(SRCDIR) -type f -name *.$(CSRCEXT))
OBJECTS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(CSRCEXT)=.o))
TESTSRC  := $(shell find $(TESTDIR) -type f -name *.$(CPPSRCEXT))
TESTOBJ  := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/$(TESTDIR)/%,$(TESTSRC:.$(CPPSRCEXT)=.o))
DEP      := $(OBJECTS:%.o=%.d) $(TESTOBJ:%.o=%.d)

# Google Test Framework variable
GTEST_DIR     = lib/gtest-1.7.0
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_   = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
CPPFLAGS     += -isystem $(GTEST_DIR)/include
GTALL   = $(BUILDDIR)/test/gtest-all.o
GTMAINO = $(BUILDDIR)/test/gtest_main.o
GTMAINA = $(BUILDDIR)/test/gtest_main.a
GTA     = $(BUILDDIR)/test/gtest.a

.SECONDEXPANSION:
.PHONY: clean test integration

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $$(dirname $@)
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(CSRCEXT)
	@echo " Building $@..."
	@mkdir -p $$(dirname $@)
	$(CC) $(CPPFLAGS) $(INC) -c -o $@ -MD -MP -MF ${@:.o=.d} $<

test: $(TEST)
	@echo " Running tests..."
	./$(TEST) $(TESTFLAGS)

$(TEST): $(TESTOBJ) $(OBJECTS) $(GTMAINA) $(LEX) $(PARSER)
	@echo " Linking test..."
	@mkdir -p $$(dirname $@)
	$(CPP) -lpthread $(LIB) $^ -o $@

$(BUILDDIR)/$(TESTDIR)/%.o: $(TESTDIR)/%.$(CPPSRCEXT) $(GTEST_HEADERS)
	@echo " Building test $@..."
	@mkdir -p $$(dirname $@)
	$(CPP) $(CPPFLAGS) $(INC) -c -o $@ -MD -MP -MF ${@:.o=.d} $<

clean:
	@echo " Cleaning...";
	$(RM) -r $(BUILDDIR) $(TARGET) $(TEST) $(INT)

# Google Tests rules
################################################################
$(GTALL): $(GTEST_SRCS_)
	@echo " Building gtest file $@..."
	@mkdir -p $$(dirname $@)
	$(CPP) $(CPPFLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(GTMAINO): $(GTEST_SRCS_)
	@echo " Building gtest file $@..."
	@mkdir -p $$(dirname $@)
	$(CPP) $(CPPFLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc -o $@

$(GTA): $(GTALL)
	@echo " Archiving gtest file $@..."
	@mkdir -p $$(dirname $@)
	$(AR) $(ARFLAGS) $@ $^

$(GTMAINA): $(GTALL) $(GTMAINO)
	@echo " Archiving gtest file $@..."
	@mkdir -p $$(dirname $@)
	$(AR) $(ARFLAGS) $@ $^

-include $(DEP)

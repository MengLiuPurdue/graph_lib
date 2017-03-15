UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
	SHAREDLIB_EXT := so
	SHAREDLIB_FLAG := -shared
endif
ifeq ($(UNAME),Darwin)
	SHAREDLIB_EXT := dylib
	SHAREDLIB_FLAG := -dynamiclib
endif

LIB := libgraph.$(SHAREDLIB_EXT)

CC := $(CXX)
CXXFLAGS := -std=c++11 -Wall -O3 -Wshadow -Werror -fPIC

SWEEPCUT_SRCS = ../sweepcut.cpp sweepcut_test.cpp
ACLPAGERANK_SRCS = ../aclpagerank.cpp aclpagerank_test.cpp
SWEEPCUT_OBJS = $(SWEEPCUT_SRCS:%.cpp=%.o)
ACLPAGERANK_OBJS = $(ACLPAGERANK_SRCS:%.cpp=%.o)

LIBOBJS = ../sweepcut.o ../aclpagerank.o

all: sweepcut aclpagerank $(LIB)

sweepcut: $(SWEEPCUT_OBJS)
	$(CC) $(SWEEPCUT_OBJS) -o sweepcut
aclpagerank: $(ACLPAGERANK_OBJS)
	$(CC) $(ACLPAGERANK_OBJS) -o aclpagerank

$(LIB): $(LIBOBJS)
	$(CC) $(SHAREDLIB_FLAG) -o $(LIB) $(LIBOBJS)


TARGET := sweepcut aclpagerank
OBJS := $(SWEEPCUT_OBJS) $(ACLPAGERANK_OBJS)

test:


.PHONY: all clean test

clean:
	$(RM) $(TARGET) $(LIB) $(OBJS)

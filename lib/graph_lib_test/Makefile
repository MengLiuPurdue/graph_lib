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
CXXFLAGS := -std=c++11 -Wall -O3 -Wshadow -fPIC

SWEEPCUT_SRCS = ../sweepcut.cpp sweepcut_test.cpp
ACLPAGERANK_SRCS = ../aclpagerank.cpp aclpagerank_test.cpp 
MQI_SRCS = ../MQI.cpp MQI_test.cpp ../maxflow.cpp ../MQI_c_interface.cpp readData.cpp
PPR_PATH_SRCS = ../ppr_path.cpp ppr_path_test.cpp ../ppr_path_c_interface.cpp readData.cpp
SWEEPCUT_OBJS = $(SWEEPCUT_SRCS:%.cpp=%.o)
ACLPAGERANK_OBJS = $(ACLPAGERANK_SRCS:%.cpp=%.o)
MQI_OBJS = $(MQI_SRCS:%.cpp=%.o)
PPR_PATH_OBJS = $(PPR_PATH_SRCS:%.cpp=%.o)

LIBOBJS = ../sweepcut.o ../aclpagerank.o ../MQI_c_interface.o ../ppr_path_c_interface.o

all: sweepcut aclpagerank MQI ppr_path $(LIB)

sweepcut: $(SWEEPCUT_OBJS)
	$(CC) $(SWEEPCUT_OBJS) -o sweepcut
aclpagerank: $(ACLPAGERANK_OBJS)
	$(CC) $(ACLPAGERANK_OBJS) -o aclpagerank
MQI: $(MQI_OBJS)
	$(CC) $(MQI_OBJS) -o MQI
ppr_path: $(PPR_PATH_OBJS)
	$(CC) $(PPR_PATH_OBJS) -o ppr_path

$(LIB): $(LIBOBJS)
	$(CC) $(SHAREDLIB_FLAG) -o $(LIB) $(LIBOBJS)


TARGET := sweepcut aclpagerank MQI ppr_path
OBJS := $(SWEEPCUT_OBJS) $(ACLPAGERANK_OBJS) $(MQI_OBJS) $(PPR_PATH_OBJS)

test:
	./sweepcut
	./aclpagerank
	./MQI
	./ppr_path


.PHONY: all clean test

clean:
	$(RM) $(TARGET) $(LIB) $(OBJS)

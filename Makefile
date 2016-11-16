OPENCV_INC=`pkg-config --cflags opencv`
OPENCV_LIBS=`pkg-config --libs opencv`

LIBS=$(OPENCV_LIBS)

OBJ_PATH=objs/
SRC_PATH=src/

AVOID=no.cpp

SRCS=$(filter-out $(AVOID), $(wildcard $(SRC_PATH)*.cpp)) 
INCLUDES=-Iinclude/ $(OPENCV_INC)
OBJS=$(patsubst $(SRC_PATH)%.cpp, $(OBJ_PATH)%.o, $(SRCS))

EXAMPLES=$(patsubst %.cpp, %, $(wildcard *.cpp))
CXX=g++
CXXFLAGS += -O2 -Wall -g


PHONY: all

all: $(EXAMPLES)


$(OBJS): objs/%.o : src/%.cpp include/%.h
	@echo Compiling obj $@
	$(CXX) $(CXXFLAGS) -o $@ -c $(CXXFLAGS) $(INCLUDES) $<

$(EXAMPLES): % : $(OBJS)
	$(CXX) -o $@  $(CXXFLAGS) $(LIBDIRS) $< $(LIBS) 
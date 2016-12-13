OPENCV_INC=`pkg-config --cflags opencv`
OPENCV_LIBS=`pkg-config --libs opencv`
OPENCV_VERSION=`pkg-config --modversion opencv | sed 's/^\(.\).*/\1/'`
LIBS=$(OPENCV_LIBS) -lpthread

OBJ_PATH=objs/
SRC_PATH=src/

AVOID=no.cpp

SRCS=$(filter-out $(AVOID), $(wildcard $(SRC_PATH)*.cpp)) 
INCLUDES=-I ./include/ $(OPENCV_INC)
OBJS=$(patsubst $(SRC_PATH)%.cpp, $(OBJ_PATH)%.o, $(SRCS))

EXAMPLES=$(patsubst %.cpp, %, $(wildcard *.cpp))

#CXX=clang++
CXX=g++
CXXFLAGS += -O2 -Wall -g -D"__OCV_VER__=$(OPENCV_VERSION)"



all: $(EXAMPLES)

$(OBJS): objs/%.o : src/%.cpp include/%.h
	@echo Compiling obj $@
	$(CXX) $(CXXFLAGS) $< -o $@ -c $(CXXFLAGS) $(INCLUDES) 

$(EXAMPLES): % : $(OBJS)
	@echo $^
	$(CXX) $^ $@.cpp -o $@ $(CXXFLAGS) $(INCLUDES) $(LIBS) 

clean:
	rm $(OBJS) $(EXAMPLES)

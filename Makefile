
##
## Variables
##


LIBS = `pkg-config opencv --libs` -lpthread 
INCLUDES = -I include/ `pkg-config opencv --cflags`
.SUFFIXES: .cpp

CXXFLAGS += -O2 -Wall -g $(INCLUDES)

AVOID = server_threads_old.cpp
SRCS = $(filter-out $(AVOID) ,$(wildcard *.cpp))
OBJS = $(patsubst %.cpp,objs/%.o,$(SRCS))
EXAMPLES= $(patsubst %.cpp,%,$(SRCS))

all: $(EXAMPLES)

##
## local rules
##

$(OBJS): objs/%.o :%.cpp
	@echo Compiling obj $@
	$(CXX) $(CXXFLAGS) -o $@ -c $(CXXFLAGS) $(INCLUDEDIR)  $<

$(EXAMPLES): % : objs/%.o
	$(CXX) -o $@ $(CXXFLAGS) $(LIBDIRS) $< $(LIBS) 

clean:
	rm $(OBJS) $(EXAMPLES)

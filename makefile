CXX=clang++-14
CXXVERSION=c++2a

CXXFLAGS=-std=$(CXXVERSION) -Werror -Wsign-conversion

all: st_pipeline

st_pipeline: st_pipeline.o
	$(CXX) $(CXXFLAGS) st_pipeline.o -o st_pipeline
	
st_pipeline.o: st_pipeline.hpp st_pipeline.cpp
	$(CXX) $(CXXFLAGS) -c st_pipeline.cpp

.PHONY: clean all

clean: 
	rm -f *.o st_pipeline

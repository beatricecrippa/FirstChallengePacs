#folder with include and lib directories for json and muparser
LIB_ROOT = ../../pacs-examples/Examples

CXX      ?= g++
CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -Wno-conversion-null -Wno-deprecated-declarations -I${LIB_ROOT}/include


EXEC = main
LDFLAGS ?= -L${LIB_ROOT}/lib
LIBS  ?= -lmuparser


all: $(EXEC)

%.o: %.cpp 
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXEC): %: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@


clean:
	$(RM) *.o

distclean: clean
	$(RM) *~ $(EXEC)

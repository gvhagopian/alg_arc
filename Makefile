
EXE = algarc

CXX        ?= g++
CXXDEBUG    = -g -Wall
CXXSTD      = -std=c++14
CXXFLAGS    = -O0  $(CXXDEBUG) $(CXXSTD)

OBJS        = \
    parser.o \
    lexer.o \
    main.o \
    graph.o \
    fn_driver.o

LIBS        = \
    -lsfml-graphics \
    -lsfml-window \
    -lsfml-system

CLEANLIST =  $(OBJS) \
    fn_parser.tab.cc fn_parser.tab.hh \
    location.hh position.hh \
    stack.hh fn_parser.output \
    fn_lexer.yy.cc $(EXE)

all: algarc

algarc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) $(LIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp
    
fn_driver.o: fn_driver.cpp fn_driver.hpp
	$(CXX) $(CXXFLAGS) -c -o fn_driver.o fn_driver.cpp

graph.o: graph.cpp graph.hpp
	$(CXX) $(CXXFLAGS) -c -o graph.o graph.cpp

parser.o: fn_parser.yy
	bison -d -v fn_parser.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o fn_parser.tab.cc

lexer.o: fn_lexer.l fn_scanner.hpp
	flex --outfile=fn_lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c fn_lexer.yy.cc -o lexer.o

clean:
	rm -rf $(CLEANLIST)


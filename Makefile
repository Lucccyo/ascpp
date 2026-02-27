CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: main test

main: main.o sign.o expr.o parser.o
	$(CXX) $(CXXFLAGS) -o main main.o sign.o expr.o parser.o

test: test_sign.o sign.o
	$(CXX) $(CXXFLAGS) -o test test_sign.o sign.o

main.o: main.cpp sign.hpp expr.hpp parser.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

test_sign.o: test_sign.cpp sign.hpp
	$(CXX) $(CXXFLAGS) -c test_sign.cpp

sign.o: sign.cpp sign.hpp
	$(CXX) $(CXXFLAGS) -c sign.cpp

parser.o: parser.cpp parser.hpp
	$(CXX) $(CXXFLAGS) -c parser.cpp

clean:
	rm -f *.o main test

run: main
	./main

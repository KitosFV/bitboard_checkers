# all: parser.o main.o
# 	g++ parser.o main.o -o ./App

# parser.o: parser.cpp parser.h
# 	g++ parser.cpp -c

# main.o: main.cpp
# 	g++ main.cpp -c

# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

# CC = gcc                        # compiler to use

LINKERFLAG = 
CXXFLAGS = -Wall

SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%.o)

all: ${BINS}
	g++ ${BINS} -o ./App -std=gnu++17

# %: %.o 
# 	@echo "Checking.."
# 	${CXX} ${LINKERFLAG} $< -o $@

%.o: %.cpp %.h
	@echo "Creating object.."
	${CXX} -c $< -std=gnu++17

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}

run: App ${BINS}
	@echo "Running..."
	./App
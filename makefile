ifeq ($(shell uname -s), Darwin)
	CXX = clang++
	CXXFLAGS += -Ofast
else
	CXX = g++
	CXXFLAGS += -O3
endif

all: src/tsp.cpp
	${CXX} src/tsp.cpp ${CXXFLAGS} -o tsp

debug: src/tsp.cpp
	${CXX} -g src/tsp.cpp -o tsp

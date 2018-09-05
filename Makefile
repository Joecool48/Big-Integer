CXX=g++
all: bigint
bigint: main.cpp src/bigint.cpp include/bigint.h
	$(CXX) main.cpp src/bigint.cpp include/bigint.h -o bigint
clean:
	rm bigint 

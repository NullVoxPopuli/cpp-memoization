all: main.cpp
	g++ -std=c++11 -o memo main.cpp
clean:
	rm memo
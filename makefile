all: worddice

worddice: worddice.cpp
	g++ -std=c++11 -o worddice worddice.cpp

.PHONY: clean
clean:
	rm -rf worddice

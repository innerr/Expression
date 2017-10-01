.PHONY: all

all:
	g++ --std=c++11 -O3 main.cpp -o expression -I rapidjson/include

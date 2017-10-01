.PHONY: all

all:
	g++ --std=c++11 main.cpp -O2 -o expression -I rapidjson/include

.PHONY: all

all:
	g++ --std=c++11 main.cpp -o expression -I rapidjson/include

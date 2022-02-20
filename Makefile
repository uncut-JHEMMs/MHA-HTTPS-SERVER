all:
	clang++ main.cpp -std=c++20 $(shell pkg-config --cflags --libs libhttpserver) -lssl -lcrypto -pthread -o net.out

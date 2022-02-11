all:
	clang++ check.cpp $(shell pkg-config --cflags --libs libhttpserver) -o net.out

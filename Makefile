all:
	clang++ loggers/PerformanceLogger.cpp main.cpp  -std=c++20 $(shell pkg-config --cflags --libs libhttpserver) -ltinyxml -lssl -lcrypto -pthread -o net.out

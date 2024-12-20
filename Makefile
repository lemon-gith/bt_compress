.PHONY: clean
all: btc test

btc: bt_compress

test: test_main

bt_compress: bt_compress.o builder.o
	g++ -std=c++23 bt_compress.o builder.o -o bt_compress

bt_compress.o: src/bt_compress.cpp
	g++ -std=c++23 -c src/bt_compress.cpp

builder.o: src/builder.cpp
	g++ -std=c++23 -c src/builder.cpp

test_main: builder.o test_main.o
	g++ -std=c++23 builder.o test_main.o -o test_main

test_main.o: test/main.cpp
	g++ -std=c++23 -c test/main.cpp -o test_main.o

clean:
	rm *.o bt_compress test_main
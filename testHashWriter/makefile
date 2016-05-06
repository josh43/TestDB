C = gcc
CPP=g++;
CPPFlags=std=c++11 -c
CFLAGS  = -g -std=c99 -Wall


all:
	main link
main: main.cpp Server.h BTree.h SimpleBtree.h
	$(CPP) main.o -o main
main.o: main.cpp Server.h BTree.h SimpleBtree.h
	$(CPP) $(CPPFlags) main.cpp
link : testHashWriter/link.c
	$(C) $(CFLAGS) testHashWriter/link.c -o link
clean :
	rm  main *.olink prop

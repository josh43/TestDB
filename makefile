C = gcc
CPP=g++
CPPFlags=-std=c++11
CFLAGS  = -g -std=c99 -Wall


SimpleTreeTester: SimpleTreeTester.cpp SimpleBtree.h
	$(CPP) $(CPPFlags) SimpleTreeTester.cpp -o SBT
main: main.cpp BTree.h SimpleBtree.h
	$(CPP) $(CPPFlags) main.cpp -o main
link : testHashWriter/link.c
	$(C) $(CFLAGS) testHashWriter/link.c -o link
clean :
	rm  main *.olink prop SBT

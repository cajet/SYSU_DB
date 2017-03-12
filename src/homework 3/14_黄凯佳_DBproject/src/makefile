All = BNode.o BTree.o queue.o ReadData.o main.o
medrank : $(All)
	g++ -o medrank $(All)
BTree.o : BTree.h BTree.cpp BNode.h ReadData.h queue.h
	g++ -c -std=c++0x BTree.cpp
queue.o : queue.h queue.cpp
	g++ -c -std=c++0x queue.cpp
ReadData.o : ReadData.h ReadData.cpp
	g++ -c -std=c++0x ReadData.cpp
main.o : main.cpp BTree.h
	g++ -c -std=c++0x main.cpp
clean :
	rm medrank $(All)

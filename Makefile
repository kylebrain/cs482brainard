all: BFS DFS IDS

BFS: bfs.o
	g++ bfs.o -o BFS

bfs.o: Romania.cpp
	g++ Romania.cpp -c -DIMPLENTING_BFS -o bfs.o

DFS: dfs.o
	g++ dfs.o -o DFS

dfs.o: Romania.cpp
	g++ Romania.cpp -c -DIMPLEMENTING_DFS -o dfs.o

IDS: ids.o
	g++ ids.o -o IDS

ids.o: Romania.cpp
	g++ Romania.cpp -c -DIMPLENTING_IDS -o ids.o

clean:
	rm BFS bfs.o DFS dfs.o IDS ids.o

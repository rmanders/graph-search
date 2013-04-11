CC=g++
CCFLAGS= -Wall
OBJS= ugraph.o


gsearch: gsearch.cpp $(OBJS)
	$(CC) $(OBJS) gsearch.cpp -o gsearch

ugraph.o: ugraph.cpp undirectedgraph.h errors.h types.h ugraph.h
	$(CC) $(CCFLAGS) ugraph.cpp -c -o ugraph.o

clean:
	rm -rf *.o

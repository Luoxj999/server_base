CC=g++
CFLAGS=-pthread -std=c++11

main: common.o thread.o threadpool.o main.o
	$(CC) $(CFLAGS) $^ -o $@ 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o main

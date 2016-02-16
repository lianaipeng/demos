all: clear test
clear:
	make clean
test:
	g++ -o test main.cpp redis_pool.cpp redis_util.cpp -lhiredis -lpthread
clean:
	rm -rf *.o test

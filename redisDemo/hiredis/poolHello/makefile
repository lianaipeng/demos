all: clear test
clear:
	make clean
test:
	g++ -o test main.cpp redis_pool.cpp -lhiredis
clean:
	rm -rf *.o test

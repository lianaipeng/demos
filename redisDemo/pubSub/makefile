all: subscribe publish 
subscribe:
	g++ -o subscribe Subscribe.cpp -lhiredis	
publish:
	g++ -o publish Publish.cpp -lhiredis	
clean:
	rm -rf subscribe publish *.o

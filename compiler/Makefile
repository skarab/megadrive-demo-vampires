all: compiler.o slz.o
	g++ compiler.o slz.o -o compiler

compiler.o: compiler.cpp
	g++ -c compiler.cpp -o compiler.o

slz.o: slz/slz.cpp
	g++ -c slz/slz.cpp -o slz.o

clean:
	rm -rf *.o

mrproper: clean
	rm -rf compiler


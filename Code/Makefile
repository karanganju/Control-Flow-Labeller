all: main.o parser.o condenser.o draw.o
	g++ -o cg_label main.o parser.o condenser.o draw.o

main.o: main.cpp parser.h condenser.h draw.h
	g++ -c main.cpp parser.h condenser.h draw.h

parser.o: parser.cpp parser.h
	g++ -c parser.cpp parser.h

condenser.o: condenser.cpp condenser.h
	g++ -c condenser.cpp condenser.h

draw.o: draw.cpp draw.h
	g++ -c draw.cpp draw.h

clean:
	rm -f *.o  *.gch

cleaner: clean
	rm -f cg_label graph.dot graph.ps

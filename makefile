COPT =  -g -Wall -Wno-sign-compare -O -fPIC -fexceptions -DNO_CPLEX # flags #-DNDEBUG
CLNFLAGS = -L/home/pashadag/lib/ -lm -lpthread
CC    = g++


# Default target
all: myjoin filter_genome ins_sort add_commas
	@echo "compilation done"

acc_commas: add_commas.o
	$(CC) acc_commas.o -o add_commas
add_commas.o : add_commas.cpp
	$(CC) -c $(COPT) add_commas.cpp -o add_commas.o
ins_sort: ins_sort.o
	$(CC) ins_sort.o -o ins_sort
ins_sort.o : ins_sort.cpp
	$(CC) -c $(COPT) ins_sort.cpp -o ins_sort.o
myjoin: myjoin.o
	$(CC) myjoin.o -o myjoin 
myjoin.o : myjoin.cpp
	$(CC) -c $(COPT) myjoin.cpp -o myjoin.o
filter_genome: filter_genome.o
	$(CC) filter_genome.o -o filter_genome 
filter_genome.o : filter_genome.cpp
	$(CC) -c $(COPT) filter_genome.cpp -o filter_genome.o


# Target deleting unwanted files
clean:
	rm -f *.o *~ 

COPT =  -Wall -Wno-sign-compare -O2 -fPIC -fexceptions 
#COPT =  -g -Wall -Wno-sign-compare -fPIC -fexceptions 


# redefine inbuilt macros and define new ones
CC    = g++
CFLAGS = $(COPT) 



# Default target
all: dbt grools
#	@echo "compilation done"

debug: CC += -g
debug: all

dbt: dbt.o
	$(CC) dbt.o -o dbt
dbt.o : dbt.cpp collapsed_db_from_genome.h
	$(CC) -c $(COPT) dbt.cpp -o dbt.o
grools: grools.o
	$(CC) grools.o -o grools
grools.o : grools.cpp graph.h
	$(CC) -c $(COPT) grools.cpp -o grools.o

# Target deleting unwanted files
clean:
	rm -f *.o *~ core mppcore

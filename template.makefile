COPT =  -Wall -Wno-sign-compare -O3 -fPIC -fexceptions 

# redefine inbuilt macros and define new ones
CC    = g++



# Default target
all: ut 
#	@echo "compilation done"

debug: CC += -g
debug: all

ut: ut.o
	$(CC) ut.o -o ut
ut.o : ut.cpp 
	$(CC) -c $(COPT) ut.cpp -o ut.o

# Target deleting unwanted files
clean:
	rm -f *.o *~ ut core mppcore

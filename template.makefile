COPT =  -Wall -Wno-sign-compare -O3 -fPIC -fexceptions 
#COPT =  -g -Wall -Wno-sign-compare -fPIC -fexceptions 

# redefine inbuilt macros and define new ones
CC    = g++
CFLAGS = $(COPT) 


#.SUFFIXES:       # remove inbuilt definitions
#.SUFFIXES: .cpp .o # define default compilation procedure
#.cpp.o:            # .o files depend on .c files
#	$(CC) $(CFLAGS) $*.c # start with a tab character!!

# Default target
all: ut 
#	@echo "compilation done"

ut: ut.o
	$(CC) ut.o -o ut
ut.o : ut.cpp 
	$(CC) -c $(COPT) ut.cpp -o ut.o

# Target deleting unwanted files
clean:
	rm -f *.o *~ ut core mppcore

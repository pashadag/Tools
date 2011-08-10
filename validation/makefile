SYSTEM     = x86-64_RHEL3.0_3.2
LIBFORMAT  = static_pic


#COPT =  -g -Wall -Wno-sign-compare -O3 -fPIC -fexceptions -DNO_CPLEX # flags #-DNDEBUG



# This is a comment - everything to the end of the line is ignored
SOURCES = graph_builder.cpp stage2.cpp
FLOWFILES = cs2.c parser_cs2.c types_cs2.h
INCLUDEFILES = union.h graph.h iofunct.h dbIterator.h defs.h collapse.cpp logClass.h optionsClass.h
OBJECTS = $(SOURCES:.cpp=.o) # Create a list of the object files using macro substitutions

# redefine inbuilt macros and define new ones
CC    = g++
COPT =  -Wall -Wno-sign-compare -O3 -fPIC -fexceptions # flags #-DNDEBUG
CFLAGS = $(COPT) #-I/home/pashadag/include/mysql++/ -I/usr/include/mysql 


#.SUFFIXES:       # remove inbuilt definitions
#.SUFFIXES: .cpp .o # define default compilation procedure
#.cpp.o:            # .o files depend on .c files
#	$(CC) $(CFLAGS) $*.c # start with a tab character!!

# Default target
#all: find_concordant cluster_indels
#	@echo "compilation done"

bestOverlap: bestOverlap.o
	$(CC) bestOverlap.o -o bestOverlap
bestOverlap.o : bestOverlap.cpp
	$(CC) -c $(COPT) bestOverlap.cpp -o bestOverlap.o


# Target deleting unwanted files
clean:
	rm -f *.o *~ graph_builder stage2 core mppcore

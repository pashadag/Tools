SYSTEM     = x86-64_RHEL3.0_3.2
LIBFORMAT  = static_pic


COPT =  -Wall -Wno-sign-compare -O2 -fPIC -fexceptions 
#COPT =  -g -Wall -Wno-sign-compare -fPIC -fexceptions 



# This is a comment - everything to the end of the line is ignored
#SOURCES = source1.c source2.c sourceN.c # List of source code
SOURCES = extractkmers.cpp 
INCLUDEFILES = defs.kh

# Create a list of the object files using macro substitutions
OBJECTS = $(SOURCES:.cpp=.o)

# redefine inbuilt macros and define new ones
CC    = g++
CFLAGS = $(COPT) 


#.SUFFIXES:       # remove inbuilt definitions
#.SUFFIXES: .cpp .o # define default compilation procedure
#.cpp.o:            # .o files depend on .c files
#	$(CC) $(CFLAGS) $*.c # start with a tab character!!

# Default target
all: dbt
#	@echo "compilation done"

dbt: dbt.o
	$(CC) dbt.o -o dbt
dbt.o : dbt.cpp collapsed_db_from_genome.h
	$(CC) -c $(COPT) dbt.cpp -o dbt.o

# Target deleting unwanted files
clean:
	rm -f *.o *~ core mppcore

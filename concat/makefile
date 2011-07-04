# This is a comment - everything to the end of the line is ignored
#SOURCES = source1.c source2.c sourceN.c # List of source code
SOURCES = main.cpp 

# Create a list of the object files using macro substitutions
OBJECTS = $(SOURCES:.c=.o)

# redefine inbuilt macros and define new ones
#CC   = gcc -c # Use gcc as the default C compiler
CC   = g++ -c # Use gcc as the default C compiler
CFLAGS = -O   # flags
LD   = g++ -o # Use gcc as the default C compiler
LIBS =        # Add any libraries required

.SUFFIXES:       # remove inbuilt definitions
.SUFFIXES: .cpp .o # define default compilation procedure
.cpp.o:            # .o files depend on .c files
	$(CC) $(CFLAGS) $*.c # start with a tab character!!

# Default target
all: concat

# Actual target and dependencies
concat: $(SOURCES)
	$(LD) $@ $(OBJECTS) $(LIBS)
	@echo "compilation done"

#source1.c: includes.h
#source2.c: includes.h

# Target deleting unwanted files
clean:
	rm -f *.o *~ concat core mppcore

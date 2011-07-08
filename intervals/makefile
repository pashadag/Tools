SYSTEM     = x86-64_RHEL3.0_3.2
LIBFORMAT  = static_pic


COPT =  -g -Wall -Wno-sign-compare -O -fPIC -fexceptions 



# This is a comment - everything to the end of the line is ignored
SOURCES = intRemove2.cpp intRemove.cpp intIntersect.cpp intSub.cpp intJoin.cpp 
INCLUDEFILES = interval.h

# Create a list of the object files using macro substitutions
OBJECTS = $(SOURCES:.cpp=.o)

# redefine inbuilt macros and define new ones
CC    = g++


#.SUFFIXES:       # remove inbuilt definitions
#.SUFFIXES: .cpp .o # define default compilation procedure
#.cpp.o:            # .o files depend on .c files
#	$(CC) $(CFLAGS) $*.c # start with a tab character!!

# Default target
all: linkHit intContain intContain2 intRemove intRemove2 intIntersect intIntersect2 intSub intJoin 
	@echo "compilation done"

linkHit: linkHit.o
	$(CC) linkHit.o -o linkHit
linkHit.o : linkHit.cpp
	$(CC) -c $(COPT) linkHit.cpp -o linkHit.o
intIntersect2: intIntersect2.o
	$(CC) intIntersect2.o -o intIntersect2
intIntersect2.o : intIntersect2.cpp
	$(CC) -c $(COPT) intIntersect2.cpp -o intIntersect2.o
intContain: intContain.o
	$(CC) intContain.o -o intContain
intContain.o : intContain.cpp
	$(CC) -c $(COPT) intContain.cpp -o intContain.o
intContain2: intContain2.o
	$(CC) intContain2.o -o intContain2
intContain2.o : intContain2.cpp
	$(CC) -c $(COPT) intContain2.cpp -o intContain2.o
intRemove2: intRemove2.o
	$(CC) intRemove2.o -o intRemove2
intRemove2.o : intRemove2.cpp
	$(CC) -c $(COPT) intRemove2.cpp -o intRemove2.o
intRemove: intRemove.o
	$(CC) intRemove.o -o intRemove
intRemove.o : intRemove.cpp
	$(CC) -c $(COPT) intRemove.cpp -o intRemove.o
intIntersect: intIntersect.o
	$(CC) intIntersect.o -o intIntersect
intIntersect.o : intIntersect.cpp
	$(CC) -c $(COPT) intIntersect.cpp -o intIntersect.o
intSub: intSub.o
	$(CC) intSub.o -o intSub
intSub.o : intSub.cpp
	$(CC) -c $(COPT) intSub.cpp -o intSub.o
intJoin: intJoin.o
	$(CC) intJoin.o -o intJoin 
intJoin.o : intJoin.cpp
	$(CC) -c $(COPT) intJoin.cpp -o intJoin.o


# Target deleting unwanted files
clean:
	rm -f *.o *~  core mppcore

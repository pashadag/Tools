
CFLAGS= -g -Wall

all : varsim

varsim : varsim.o
	g++ ${CFLAGS} varsim.o -o varsim -lm

%.o : %.cpp
	g++ ${CFLAGS} -c $< -o $@ 

clean :
	rm -f *.o varsim


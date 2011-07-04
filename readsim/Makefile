
CFLAGS= -g -Wall

all : sequence_genome 

sequence_genome : mylib.o fasta.o sequencing.o sequence_genome.o 
	gcc ${CFLAGS} -o sequence_genome $^ -lm 

#split_file : ./biolib/mylib.o split_file.o
#gcc ${CFLAGS} -o split_file $^ -lm 

%.o : %.c
	gcc ${CFLAGS} -c $< -o $@ 

clean :
	rm -f *.o *.exe a.out
	rm -f ./bin/*
	rm -f ./biolib/*.o

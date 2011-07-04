/*
 * fasta.c
 *
 *  Created on: May 8, 2009
 *      Author: mfiume
 */

#include "fasta.h"
#include "mylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * CONSTANTS
 */

#define INIT_GENOME_SIZE 100	// initial genome size
#define MAXLINE 100			// maximum line length
#define GROWTH_FACTOR 2		// factor at which genome size grows
#define FASTA_LINE_LENGTH 60

Fasta *f;					// Fasta data structure

int resizeGenome(int genome_array_size, int new_pos) {

	/* resize genome if it's been overgrown */
	if (new_pos > genome_array_size) {

		// determine new genome size
		int new_genome_array_size;
		if (genome_array_size == 0) {

			new_genome_array_size = INIT_GENOME_SIZE;

			// allocate space for initial genome
			f->genome->sequence = (char *)malloc(sizeof(char)*new_genome_array_size);

		} else {
			new_genome_array_size = genome_array_size*GROWTH_FACTOR;

			// allocate space for new genome
			f->genome->sequence = (char *)realloc(f->genome->sequence,sizeof(char)*new_genome_array_size);
		}

		// return the size of the new genome array
		return new_genome_array_size;
	}

	return genome_array_size;
}

void appendToGenome(char *linep) {

	// current position in the genome
	static int genome_array_size = 0;

	// how long this line is
	int linelength = strlen(linep);

	// current position, after considering this line
	int new_pos = f->genome->size + linelength;

	// resize the genome if it has been overgrown
	genome_array_size = resizeGenome(genome_array_size,new_pos);

	// append the line to the genome
	int i; for (i = 0; i <  linelength; i++) {

		// copy nucleotides only (not CRLFs)
		if (linep[i] != '\n' && linep[i] != '\r') {
			f->genome->sequence[f->genome->size+i] = linep[i];
		} else { new_pos--; }
	}

	// set the new current position
	f->genome->size = new_pos;
}

void initFastaGenome() {
	f = (Fasta *)malloc(sizeof(Fasta));
	f->genome = (Genome *)malloc(sizeof(Genome));
	f->genome->size = 0;
	f->header[0] = '\0';
}

Fasta * fastaGenome(FILE *fastafilep) {

	// initialize the genome
	initFastaGenome();

	// a line from the input file
	char line[MAXLINE];

	// number of lines read
	int numlines = 0;

	// append each line to the genome data structures
	while (!feof(fastafilep)) {
		line[0] = '\0';
		numlines++;
		fgets(line,MAXLINE,fastafilep);
		remchr(line,'\n');
		remchr(line,'\r');
		if (numlines == 1) { strncpy(f->header,line,MAXLINE); }
		else { appendToGenome(line); }
	}

	// resize genome to the final size
	f->genome->sequence = (char *)realloc(f->genome->sequence,sizeof(char)*f->genome->size);

	// return fasta struct
	return f;
}

void printGenome(Fasta * f, FILE * fp) {

	fprintf(fp,"%s\n",f->header);

	int cnt = 0;
	while (f->genome->size != 0) {
		if (cnt++ % FASTA_LINE_LENGTH == 0) { fprintf(fp,"\n"); }
		fprintf(fp,"%c",f->genome->sequence[0]);
		f->genome->sequence = f->genome->sequence + 1;
		f->genome->size = f->genome->size - 1;
	}

	fprintf(fp,"\n");

	return;
}

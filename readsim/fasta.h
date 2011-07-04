/*
 * fasta.h
 *
 *  Created on: May 8, 2009
 *      Author: mfiume
 */

#ifndef FASTA_H_
#define FASTA_H_

#include <stdio.h>

struct genome {
	char *sequence;
	int size;
	char *description;
};

typedef struct genome Genome;

struct fastaGenome {
	char header[100];
	struct genome * genome;
};

typedef struct fastaGenome Fasta;

Fasta * fastaGenome(FILE *fp);
void printGenome(Fasta * f, FILE * fp);

#endif /* FASTA_H_ */

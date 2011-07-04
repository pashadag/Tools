/*
 * sequencing.h
 *
 *  Created on: May 26, 2009
 *      Author: fiume
 */

#ifndef SEQUENCING_H_
#define SEQUENCING_H_

#include "fasta.h"

struct hit {
	int start;
	int end;
	double score;
	char strand;
	char errors[40];
	int numErrors;
};

typedef struct hit Hit;

struct read {
	char id[40];
	char * sequence;
	char direction;
	unsigned int length;
	double quality;
	Hit ** hits;
	int num_hits;
};

typedef struct read Read;

struct matepair {
	char templateid[20];
	Read * forward;
	Read * reverse;
	int insert_size;
};

typedef struct matepair Matepair;


void revcomp(char * s, int slen);
Read * generateRead(Genome * g, int read_len, char * templateid, char direction, char strand);
Read * simulateReadErrors(Read * r, double subrate);
Read * generateReadWithErrors(Genome * g, int read_len, char * templateid, char direction, double subrate);
Matepair * generateMatepair(Genome * g, char * templateId, int read_len, double mu_insert_size, double stdev_insert_size);
Matepair * generateMatepairWithErrors(Genome * g, char * templateId, int read_len, double mu_insert_size, double stdev_insert_size, double subrate, int tech);

void printRead(Read * r, FILE * fp);
void readToProbcalc(Read * r, FILE *fp);

void destructMatepair(Matepair * m);

#endif /* SEQUENCING_H_ */

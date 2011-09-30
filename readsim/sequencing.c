
#include "mylib.h"
#include "fasta.h"
#include "sequencing.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


int technology;

void revcomp(char * s, int slen) {
	int i;
	char c;
	int halfway = slen / 2; //this is an automatic floor
	for (i = 0; i < halfway; i++) {
		c = s[i];
		s[i] = s[slen - i -1];
		s[slen - i -1] = c;
	}
	for (i = 0; i < slen; i++) {
		if (s[i] == 'A') s[i]='T'; 
		else if (s[i]=='C') s[i] = 'G';
		else if (s[i]=='G') s[i] = 'C';
		else if (s[i]=='T') s[i] = 'A';
		else if (s[i] == 'a') s[i]='t'; 
		else if (s[i]=='c') s[i] = 'g';
		else if (s[i]=='g') s[i] = 'c';
		else if (s[i]=='t') s[i] = 'a';
	}
	return;
}


Read * generateRead(Genome * g, int read_len, char * templateid, char direction, char strand) {

	// generated read
	Read * r = (Read *)malloc(sizeof(Read));
	r->length = read_len;
	r->quality = 1.0;
	r->direction = direction;

	// generate read sequence from genome
	int start = randInt(0,g->size - read_len);
	r->sequence = str(read_len);
	strncat(r->sequence, g->sequence+start,read_len);
	if (strand == '-') revcomp(r->sequence, read_len);

	// create the 'perfect' hit record
	Hit * h = (Hit *)malloc(sizeof(Hit));
	h->start = start;
	h->end = start + read_len - 1;
	h->score = 1;
	h->strand = strand;
	h->numErrors = 0;
	strcpy(h->errors, "");

	// add the 'perfect' hit to the hit list
	r->num_hits = 1;
	r->hits = (Hit **)malloc(sizeof(Hit *));
	r->hits[0] = h;

	sprintf(r->id,"%s",templateid);
	/*
	if (technology == 0) {
		sprintf(r->id,"%s_%c3",templateid,direction);
	} else if (technology == 1) {
		char c = (direction == 'F') ? '1' : '2';
		sprintf(r->id,"%s/%c",templateid,c);
		//sprintf(r->id,"%s_%d_%c_%d/%c", templateid, r->hits[0]->start, r->hits[0]->strand, r->hits[0]->numErrors, c);
	} else {
		fprintf(stderr, "Internal error: technology = %i.\n", technology);
		exit(1);
	}
	*/


	// return the read
	return r;
}



Read * simulateReadErrors(Read * r, double subrate) {

	char nucs[] = "ACTG";

	if (subrate < 0 || subrate > 1) {
		fprintf(stderr,"error: read error rate must be between 0 and 1\n");
		return r;
	}

	double d;

	//fprintf(stderr, "\nBEFORE: %s, %s.\n", r->id, r->hits[0]->errors);

	int i; for (i = 0; i < r->length; i++) {
		d = randDouble (0, 1);
		if ((d < subrate) && (r->sequence[i] != 'N')) { 
			int j;
			do {
				j = randInt(0,4);
			} while (nucs[j] == toupper(r->sequence[i]));

			r->sequence[i] = nucs[j]; 

			//record the changes in hit record
			char loc[20]; sprintf(loc, "%i%c,", i, nucs[j]); 
			strncat(r->hits[0]->errors,loc, 20);
			r->hits[0]->numErrors++;
		}
	}
	//fprintf(stderr, "\nAFTER: %s, %s.\n", r->id, r->hits[0]->errors);

	return r;
}

Matepair * generateMatepairWithErrors(Genome * g, char * templateId, int read_len, double mu_insert_size, double stdev_insert_size, double subrate, int tech) {
	technology=tech;

	Matepair * m = generateMatepair(g, templateId, read_len, mu_insert_size, stdev_insert_size);
	m->forward = simulateReadErrors(m->forward, subrate);
	m->reverse = simulateReadErrors(m->reverse, subrate);

	return m;
}



Matepair * generateMatepair(Genome * g, char * templateId, int read_len, double mu_insert_size, double stdev_insert_size) {

	// whether or not a valid matepair is generated
	// (i.e. one whose reads lie within the genome)
	int valid_matepair = 0;

	// matepair
	Matepair * m = (Matepair *)malloc(sizeof(Matepair));
	strncpy(m->templateid,templateId,20);

	// forward and reverse reads
	Read * f; Read * r;
	int insert_size;

	// generate a valid matepair
	while(!valid_matepair) {

		int i = randInt(0,2);
		char strand;
		if (i == 0) strand = '+'; else strand = '-';
		
		r = generateRead(g,read_len,m->templateid,'R', strand);


		// sample insert size from Gaussian
		insert_size = floor(randNorm(mu_insert_size,stdev_insert_size));
		int f_start, f_end;
		if ((strand == '+') || (technology == 0)) {
			f_start = r->hits[0]->start + insert_size - read_len;
			f_end = f_start + read_len - 1;
		} else {
			f_start = r->hits[0]->end - insert_size + 1;
			f_end = f_start + read_len - 1;
		}


		if ((f_start >= 0) && (f_end < g->size)) {
			valid_matepair = 1;

			// create the reverse read
			f = (Read *)malloc(sizeof(Read));
			f->direction = 'F';

			sprintf(f->id,"%s",m->templateid);
			 if (technology == 1) { 
				 if (strand == '+') strand = '-'; else strand = '+'; //change strand for solexa
			 }
			/*if (technology == 0) {
				sprintf(f->id,"%s_%c3",m->templateid,f->direction);
			} else if (technology == 1) {
				sprintf(f->id,"%s/1",m->templateid);
				if (strand == '+') strand = '-'; else strand = '+'; //change strand for solexa
			} else {
				fprintf(stderr, "Internal error: technology = %i.\n", technology);
				exit(1);
			}
			*/

			f->sequence = str(read_len);
			f->quality = 1.0;
			strncat(f->sequence,g->sequence+f_start,read_len);
			if (strand == '-') revcomp(f->sequence, read_len);

			f->length = read_len;
			// create the 'perfect' hit record
			Hit * h = (Hit *)malloc(sizeof(Hit));
			h->start = f_start;
			h->end = f_end;
			h->score = 1;
			h->strand = strand;
			h->numErrors = 0;
			strcpy(h->errors, "");
			// add the 'perfect' hit to the hit list
			f->num_hits = 1;
			f->hits = (Hit **)malloc(sizeof(Hit *));
			f->hits[0] = h;
		}
	}

	// create the matepair
	m->forward = f;
	m->reverse = r;
	m->insert_size = insert_size;

	return m;
}

void getReadName(Read * r, char * name) {
	if (technology == 0) {
		sprintf(name, ">%s_%s_%d_%c_%d_%c3", nameprefix, r->id, r->hits[0]->start, r->hits[0]->strand, r->hits[0]->numErrors, r->direction);
	} else if (technology == 1) {
		char c = (r->direction == 'F') ? '1' : '2';
		sprintf(name, ">%s_%s_%d_%c_%d/%c", nameprefix, r->id, r->hits[0]->start, r->hits[0]->strand, r->hits[0]->numErrors, c);
	} else {
		fprintf(stderr, "Internal error: technology = %i.\n", technology);
		exit(1);
	}
}

void printRead(Read * r, FILE * fp) { 
	char name[40];
	getReadName(r, name);
	fprintf(fp, "%s\n%s\n", name, r->sequence);
}

void destructRead(Read * r) {
	int i; for (i = 0; i < r->num_hits; i++) { free(r->hits[i]); }
	//free(r->id);
	free(r->hits);
	free(r->sequence);
	free(r);
}

void destructMatepair(Matepair * m) {
	destructRead(m->forward);
	destructRead(m->reverse);
	free(m);
}

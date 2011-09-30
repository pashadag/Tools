/*
 * sequence_genome.c
 *
 *  Created on: May 27, 2009
 *      Author: fiume
 */

#include "mylib.h"
#include "fasta.h"
#include "sequencing.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void usage(char ** argv) {
	printf("usage: %s <reference_genome> <coverage> <readlen> <mu_insert> <stdev_insert> <error rate> <reads_out_file> <locs_out_file> <technology(0=abi, 1=solexa)> <prefix>\n",argv[0]);
}

/*
 * CONSTANTS
 */

#define EXPECTED_ARGS 10 	// expected number of input arguments

int main(int argc, char ** argv) {

	/* check input args */
	if (argc != EXPECTED_ARGS+1) {
		usage(argv);
		exit(1);
	}

	srand( time(NULL) );

	FILE * outf; FILE * locoutf; FILE * report = stdout;

	fprintf(report, "\nSequence a Genome v1.0\n");
	fprintf(report, "----------------------\n\n");

	char * infilename = (char *)malloc(sizeof(char)*(strlen(argv[1])+1));
	strcpy(infilename,argv[1]);

	char * outfilename = (char *)malloc(sizeof(char)*(strlen(argv[7])+1));
	strcpy(outfilename,argv[7]);

	char * locoutfilename = (char *)malloc(sizeof(char)*(strlen(argv[8])+1));
	strcpy(locoutfilename,argv[8]);

	int technology = atoi(argv[9]);

	nameprefix = (char *)malloc(sizeof(char)*(strlen(argv[10])+1));
	strcpy(nameprefix, argv[10]);

	int coverage; if ((coverage = atoi(argv[2])) == 0) {
		fprintf(stderr,"error: parsing integral coverage from %s\n",argv[2]);
	}

	int readlen; if ((readlen = atoi(argv[3])) == 0) {
		fprintf(stderr,"error: parsing integral read length from %s\n",argv[3]);
	}

	double mu; if ((mu = atof(argv[4])) == 0) {
		fprintf(stdout,"warning: float mu from %s is either 0 or cannot parse\n",argv[4]);
	}

	double stdev; if ((stdev = atof(argv[5])) == 0) {
		fprintf(stdout,"warning: float stdev from %s is either 0 or cannot parse\n",argv[5]);
	}

	double errrate; if ((errrate = atof(argv[6])) == 0) {
		fprintf(stdout,"warning: float error rate from %s is either 0 or cannot parse\n",argv[6]);
	}
	if ((errrate < 0) || (errrate > 1)) {
		fprintf(stderr, "error: errrate must be between 0 and 1.\n");
		exit(1);
	}

	/*
	 * Print parameter
	 */
	fprintf(report, "PARAMETERS:\n");
	fprintf(report, "  Genome file: %s\n\tReading file, please wait\n", infilename);

	FILE * inp = efopen(argv[1],"r");
	Fasta * f = fastaGenome(inp);
	efclose(inp);

	int num_matepairs = ceil(coverage*( ((double) f->genome->size) / readlen )/2);

	fprintf(report, "  Genome size: %d\n", f->genome->size);
	fprintf(report, "  Desired coverage: %dx (matepairs %d)\n", coverage, num_matepairs);
	fprintf(report, "  Read lengths: %d\n", readlen);
	fprintf(report, "  Substitution rate: %f\n", errrate);
	fprintf(report, "  Insert average size: %f standard deviation: %f\n", mu, stdev);
	fprintf(report, "  Read Output file: %s\n", outfilename);
	fprintf(report, "  Location Output file: %s\n", locoutfilename);
	fprintf(report, "  Technology = %i (0=abi, 1=solexa)\n", technology);
	fprintf(report, "  Name prefix = %s\n", nameprefix);

	fprintf(report, "\n");

	outf = efopen(outfilename,"w");
	locoutf = efopen(locoutfilename,"w");

	char allns[1000];
	int i; for (i = 0; i < readlen; i++) allns[i]='N'; //initialize allns
	allns[i]='\0';  

	char templateid[20];
	int allnsCounter = 0;

	fprintf(report, "Generating matepairs, please wait\n");

	for (i = 0; i < num_matepairs; i++) {

		progress_bar(report, i+1, num_matepairs, 1);

		templateid[0] = '\0';
		int width = ceil (log(num_matepairs) / log(10.0)) ;
		sprintf(templateid,"%.*d",width,i);

		Matepair * m = generateMatepairWithErrors(f->genome, templateid, readlen, mu, stdev, errrate,technology);


		int d = 1 + max(max(m->forward->hits[0]->start, m->reverse->hits[0]->start), max(m->forward->hits[0]->end, m->reverse->hits[0]->end)) - min(min(m->forward->hits[0]->start, m->reverse->hits[0]->start), min(m->forward->hits[0]->end, m->reverse->hits[0]->end));
					
		if (strcmp(m->forward->sequence, allns) != 0) {
			printRead(m->forward,outf);
			char name[40];
			getReadName(m->forward, name);
			fprintf(locoutf, "%s\t%d\t%s\n", name, d, m->forward->hits[0]->errors); 
		} else allnsCounter++;

		if (strcmp(m->reverse->sequence, allns) != 0) {
			printRead(m->reverse,outf);
			char name[40];
			getReadName(m->reverse, name);
			fprintf(locoutf, "%s\t%d\t%s\n", name, d, m->forward->hits[0]->errors); 
		} else allnsCounter++;

		destructMatepair(m);
	}

	efclose(outf);

	fprintf(report, "\n\nSuppressed the output of %i reads because they were all Ns.\n\n", allnsCounter);
	fprintf(report, "Done.\n\n");

	return 0;
}

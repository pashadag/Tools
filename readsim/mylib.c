#define MAXLINE 256

#include "mylib.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define GROWTH_FACTOR 2
#define INIT_ARRAY_SIZE 10

/*
 * STRING FUNCTIONS
 */

char * chrrep(char * str, char old, char new) {
	int len = strlen(str);
	int i; for (i = 0; i < len; i++) { if (str[i] == old) { str[i] = new; } }
	return str;
}

char * remchr (char * str, char old) {

	int offset = 0;
	int len = strlen(str);

	int i; for (i = 0; i < len; i++) {
		if (str[i] == old) { offset++; }
		else { str[i-offset] = str[i]; }
	}

	str[len-offset] = '\0';
	return str;
}

/*
 * INTEGER FUNCTIONS
 */
int min(int a, int b) { if (a > b) { return b; } else { return a; } }
int max(int a, int b) { if (a > b) { return a; } else { return b; } }

char * extendStrFromPos(char * str, char * insert, int pos) {

	// allocate space for new char *
	char * new_str = (char *)malloc(sizeof(char)*(strlen(str)+strlen(insert)+1));
	new_str[0] = '\0';

	// copy from 0 to position of last .
	strncat(new_str,str,pos);

	// copy insert
	strcat(new_str,insert);

	// copy from position of last . to end
	if (pos < strlen(str)) { strcat(new_str,str + pos); }

	// return char *
	return new_str;
}

char * extendStrFromRPos(char * str, char * insert, int rpos) {
	int pos = strlen(str)-rpos;
	return extendStrFromPos(str,insert,pos);
}


char * extendStrFromChar(char * str, char * insert, char c) {
	// find position of first c
	char * ppos = strchr(str,c);

	int len_str = strlen(str);
	int pos = len_str - strlen(ppos);

	return extendStrFromPos(str,insert,pos);
}

char * extendStrFromRChar(char * str, char * insert, char c) {

	// find position of last c
	char * ppos = strrchr(str,c); /* note strRchr */

	int len_str = strlen(str);
	int pos = len_str - strlen(ppos);

	return extendStrFromPos(str,insert,pos);
}

char * basename(char * str) {
	char * p = strrchr(str,'/');

	if (p == NULL) { return str; }

	p = p + 1;
	char * result = (char *)malloc(sizeof(char)*(strlen(p)+1));
	result[0] = '\0';

	strcat(result,p);

	return result;
}

/*
 * PARSING INTERVAL FILES
 */

void printIntervals(Interval ** array, int array_size, FILE * fp) {

	int i; for (i = 0; i < array_size; i++) {
		Interval * inter = array[i];
		fprintf(fp, "chr%d %d %d %s\n", inter->chr, inter->from, inter->to, inter->description);
	}

	return;
}

void shiftIntervals(Interval ** interval_array, int array_size, int chr, int pivot, int shift) {

	//printf("shifting intervals in chr%d past %d   %d spaces",chr, pivot,shift);

	int i; for (i = 0; i < array_size; i++) {
		Interval * inter = interval_array[i];

		//printf("\tshifting: %d %d %d %s to ", inter->chr, inter->from, inter->to, inter->description);

		if (inter->chr == chr) {
			if (inter->from >= pivot) {
				inter->from = inter->from+shift;
				inter->to = inter->to+shift;
			}
		}

	}

	//printf("done shifting\n");

	return;
}

void addInterval(Interval ** interval_array, int * num_intervals, Interval * inter) {

	*num_intervals = *num_intervals + 1;
	interval_array = (Interval **)realloc(interval_array,sizeof(Interval *)*(*num_intervals));
	interval_array[(*num_intervals)-1] = inter;

	//printf("adding cnv: %d %d %d %s\n", interval_array[(*num_intervals)-1]->chr, interval_array[(*num_intervals)-1]->from, interval_array[(*num_intervals)-1]->to, interval_array[(*num_intervals)-1]->description);
}

void removeInterval(Interval ** interval_array,int * num_intervals,int interval_to_remove) {

	int num_removed = 0;

	int i; for (i = 0; i < *num_intervals; i++) {
		if (i == interval_to_remove) { num_removed++; }
		else { interval_array[i-num_removed] = interval_array[i]; }
	}

	*num_intervals = *num_intervals-1;

	interval_array = (Interval **)realloc(interval_array,sizeof(Interval *)*(*num_intervals));

	return;
}

Interval ** intervalWithFilter(FILE * fp, int * num_intervals, int filter) {

	// a line from the input file
	char line[MAXLINE];

	// number of lines read
	int numlines = 0;

	// current size of the interval array
	int array_size = INIT_ARRAY_SIZE;

	// pp to the interval array
	Interval ** interval_array = (Interval **)malloc(sizeof(Interval *)*array_size);

	// read entire file line by line
	while (!feof(fp)) {

		// initialize a new interval
		Interval * i = (Interval *)malloc(sizeof(Interval));

		numlines++;			// keep track of lines read

		line[0] = '\0'; 	// reset the line buffer

		fgets(line,MAXLINE,fp);		// read next line into buffer

		char * pch;

		pch = strtok (line," \t");

		int token_num = 0;
		int done = 0;

		int wrong_chromosome = 0;

		while (!done && pch != NULL && !wrong_chromosome) {

			token_num++;

			switch (token_num) {

				// chr token
				case 1:
					i->chr = chromosomeNumber(pch);

					if (i->chr != 0 && i->chr != filter) { wrong_chromosome = 1; }

					break;

				// from token
				case 2:
					i->from = atoi(pch)-1;
					break;

				// to token
				case 3:
					i->to = atoi(pch)-1;
					done = 1;
					break;

				default:
					break;
			} // end switch

			// get pointer to next token
		    pch = strtok (NULL, " \t");

		} // end while (done tokenizing line)

		// don't add the interval to the array if it's for
		// the wrong chromosome
		if(wrong_chromosome) { numlines--; }

		// otherwise, add the interval to the array
		else {
			if (array_size < numlines) {
				array_size = array_size*GROWTH_FACTOR;
				interval_array = (Interval **)realloc(interval_array, sizeof(Interval *)*array_size);
			}

			// add the interval to the array
			if (i->chr == 0) { numlines--; }
			else { interval_array[numlines-1] = i; }
		}

	} // end while (done reading file)

	array_size = numlines;
	interval_array = (Interval **)realloc(interval_array, sizeof(Interval *)*array_size);
	*num_intervals = array_size;

	return interval_array;
}

Interval ** interval(FILE * fp, int * num_intervals) { return intervalWithFilter(fp, num_intervals, 0); }

/*
 * OTHER STUFF
 */

/*
 * return the integer nuumber from a character string representing
 * a chromosome
 */
int chromosomeNumber(char * c) {
	if (strlen(c) > 3 && strncmp(c,"chr",3) == 0) { c = c + 3; }
	if (c[0] == 'X') { return 23; }
	else if (c[0] == 'Y') { return 24; }
	else { return atoi(c); }
}

/*
 * return 1 iff pos lies within any interval in inter_array
 */
int intersect(int pos,Interval ** inter_array, int len_of_array) {
	int i; for (i = 0; i < len_of_array; i++) {
		Interval * inter = inter_array[i];
		if (pos > inter->from && pos < inter->to) { return 1; }
	}
	return 0;
}

/*
 * Initialize an empty string
 */
char * str(int strlen) {
	char * str = (char *)malloc(sizeof(char)*strlen+1);
	str[0] = '\0';
	return str;
}

char * strFrom(char * s) {
	char * res = str(strlen(s));
	strncpy(res,s,strlen(s));
	return res;
}

/*
 * STANDARD FUNCTIONS, WITH ERROR CHECKING
 */

/*
 * open a file, with error checking
 */
FILE * efopen(const char * fn, const char * mode) {
	FILE * f;
	if (strcmp(fn, "-") == 0) {
		f = stdout;
	} else if ( (f = fopen(fn,mode)) == NULL ) {
		fprintf(stderr,"error: opening file %s\n",fn);
		exit(1);
	}
	return f;
}

/*
 * close a file with error checking
 */
void efclose(FILE * f) {
	if ((f != stdout) && (fclose(f) != 0)) {
		fprintf(stderr,"warning: could not close file\n");
	}
}

/*
 * allocate memory, with error checking
 */
void * emalloc(int nbytes) {
	void * p;

	if ((p = malloc(nbytes)) == NULL) {
		fprintf(stderr,"error: failed to allocate space\n");
		exit(1);
	}

	return p;
}

/*
 * RANDOM NUMBERS
 */

/*
 * random double
 */

double randDouble (double from, double to) {

	double r = (double) rand();
	double range = to - from;

	return (r / RAND_MAX)*range + from;
}

/*
 * random integer
 */
int randInt(int from, int to) { return floor(randDouble(from,to)); }

/*
 * random Gaussian
 */
double randNorm(double mu, double stdev) {
	double r = randDouble(0,1);
	return stdev*r + mu;
}


/*
 * print a progress bar
 */
void progress_bar(FILE *out, int at, int of, int incr) {

	static int lastperc, beenhere;
	static char whirly = '\\';
	char progbuf[52];
	int perc, i, j, dec;

	if (at == 0 && of == 0) {
		beenhere = lastperc = 0;
		whirly = '\\';
		return;
	}

	perc = (at * 100 * incr) / of;

	if (beenhere && perc == lastperc) { return; }

	beenhere = 	1;
	lastperc = perc;
	dec = perc % incr;
	perc /= incr;

	/* any excuse to have a whirly gig */

	switch (whirly) {

	case '|':
		whirly = '/';
		break;
	case '/':
		whirly = '-';
		break;
	case '-':
		whirly = '\\';
		break;
	case '\\':
		whirly = '|';
		break;
	}

	if (at >= of) { whirly = '|'; }

	progbuf[25] = whirly;

	for (i = j = 0; i <= 100; i += 2) {
		if (j != 25) {
			if (i <= perc) { progbuf[j++] = '='; }
			else { progbuf[j++] = ' '; }
		}
		else { j++; }
	}

	progbuf[51] = '\0';

	if (incr == 100) { fprintf(out, "\rProgress: [%s] %3d.%02d%%", progbuf, perc, dec); }
	else if (incr == 10) { fprintf(out, "\rProgress: [%s] %3d.%d%%", progbuf, perc, dec); }
	else { fprintf(out, "\rProgress: [%s] %3d%%", progbuf, perc); }

	fflush(out);
}

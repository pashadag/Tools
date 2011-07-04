/*
 * mylib.h
 *
 *  Created on: May 11, 2009
 *      Author: mfiume
 */

#ifndef MYLIB_H_
#define MYLIB_H_

#include <stdio.h>

/*
 * STANDARD FUNCTIONS, WITH ERROR CHECKING
 */
FILE * efopen(const char * fn, const char * mode);
void efclose(FILE * f);
void * emalloc(int nbytes);

/*
 * RANDOM NUMBERS
 */
int randInt(int from, int to);
double randDouble (double from, double to);
double randNorm(double mu, double stdev);


/*
 * INTERVALS
 */

struct interval {
	unsigned int chr;
	unsigned int from;
	unsigned int to;
	char * description;
};

typedef struct interval Interval;

void printIntervals(Interval ** array, int array_size, FILE * fp);
Interval ** interval(FILE * fp, int * num_intervals);
Interval ** intervalWithFilter(FILE * fp, int * num_intervals, int filter);
void addInterval(Interval ** interval_array, int * num_intervals, Interval * inter);
void removeInterval(Interval ** interval_array,int * num_intervals,int interval_to_remove);
void shiftIntervals(Interval ** interval_array, int array_size, int chr, int pivot, int shift);
int intersect(int pos,Interval ** inter_array, int len_of_array);

/*
 * STRING
 */

/*
 * INITIALIZATION
 */
char * str(int strlen);
char * strFrom(char * s);

/*
 * > PARSING
 */
int chromosomeNumber(char * c);

/*
 * > EDITING
 */

char * chrrep(char * str, char old, char new);
char * remchr (char * str, char old);

char * basename(char * str);
char * extendStrFromPos(char * str, char * insert, int pos);
char * extendStrFromRPos(char * str, char * insert, int rpos);
char * extendStrFromChar(char * str, char * insert, char c);
char * extendStrFromRChar(char * str, char * insert, char c);

/*
 * MATH FUNCTIONS
 */
int min(int a, int b);
int max(int a, int b);

/*
 * PROGRESS BAR
 */
void progress_bar(FILE *out, int at, int of, int incr);

#endif /* MYLIB_H_ */

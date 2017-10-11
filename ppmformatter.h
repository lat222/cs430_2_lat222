#ifndef PPMFORMATTER
#define PPMFORMATTER

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

int read_header(FILE* fp);
int read_p3(FILE* fp);
int read_p6(FILE* fp);
void write_p3(FILE* fp);
void write_p6(FILE* fp);


#endif
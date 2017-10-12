#ifndef RAYCAST
#define RAYCAST

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "linkedlist.h"
#include "helperfuncs.h"

typedef struct vector
{
	float x,y,z;
} vector;

typedef struct objectNode
{
	char type;
	pixel* pix;
	vector* position;
	int radius;
	vector* normal;

	struct objectNode* next;

} objectNode;

int main(int argc, char* argv[]);

node* raycast(FILE* fp, int width, int height);
void readObjectFile(FILE* fp, objectNode* head);
objectNode* readObject(char* line);
vector* make_unit_vector(float x, float y, float z);
objectNode* shoot(vector* rayVector, objectNode* head);
pixel* shade(objectNode* hitOject);



#endif
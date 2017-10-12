#ifndef RAYCAST
#define RAYCAST

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "helperfuncs.h"

typedef struct vector
{
	int x,y,z;
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
objectNode* readObjectFile(FILE* fp);
objectNode* readObject(char* line);
objectNode* shoot(vector* rayVector, objectNode* head);
pixel* shade(objectNode* hitOject);



#endif
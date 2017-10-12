#ifndef RAYCAST
#define RAYCAST

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "helperfuncs.h"

typedef struct vector
{
	int x;
	int y;
	int x;
} vector;

typedef struct objectNode
{
	char type;
	pixel* pix;
	vector* postion;
	int radius;
	vector* normal;

	struct nextObjectNode*;

} objectNode;


node* raycast(FILE* fp, width, height);
objectNode* readObjects(FILE* fp);
object* shoot(vector* rayVector, objectNode* head);
pixel* shade(object* hitOject);



#endif
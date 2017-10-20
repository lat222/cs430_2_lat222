#ifndef RAYCAST
#define RAYCAST

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "linkedlist.h"
#include "helperfuncs.h"

typedef double* V3;

// class to hold object information
typedef struct objectNode
{
	char type;
	V3 pix;
	V3 position;
	int radius;
	V3 normal;

	struct objectNode* next;

} objectNode;

// calls all the following functions and returns a list of pixels
node* raycast(FILE* fp, int width, int height);
// reads all of the objects in a file after the first object that is not the camera has already been read in
void readObjectFile(FILE* fp, objectNode* head);
// creates an object based off of a line in the input file
objectNode* readObject(char* line);
// makes a unit vector from args x, y, and z
/*vector* make_unit_vector(float x, float y, float z);
// returns the pixel struct of the nearest hit object
pixel* shoot(vector* rayVector, objectNode* head);
// returns the distance t if the ray vector intersected with the object
float ray_sphere_intersection(vector* rayVector, objectNode* oNode);
float ray_plane_intersection(vector* rayVector, objectNode* oNode);
// returns a dot product
float dot_product(vector* v, vector* u);*/
double get_first_vector_value(char* line);

double get_last_vector_value(char* line);

#endif
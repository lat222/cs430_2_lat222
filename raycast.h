#ifndef RAYCAST
#define RAYCAST

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "3DMath.h"

typedef struct Pixel {

	// The is where the colors for a pixel are stored
    unsigned char R, G, B;

} Pixel;

// class to hold object information
typedef struct Object
{
	char type;
	Pixel* pix;
	V3 position;
	int radius;
	V3 normal;

} Object;

// calls all the following functions and returns a list of pixels
Pixel* raycast(FILE* fp, int width, int height);

// returns the pixel struct of the nearest hit object
Pixel* shoot(V3 rayVector, Object* objects, int objectCount);
// returns the distance t if the ray vector intersected with the object
double ray_sphere_intersection(V3 rayVector, Object* object);
double ray_plane_intersection(V3 rayVector, Object* object);

int check_file_path(char* fp);

#endif
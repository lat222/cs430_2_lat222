#ifndef RAYCAST
#define RAYCAST

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define maxObjects 128
#define backgroundColorR 0
#define backgroundColorG 0
#define backgroundColorB 0

typedef double* V3;

typedef struct Pixel {

	// The is where the colors for a pixel are stored
    unsigned char R, G, B;

} Pixel;

// class to hold object information
typedef struct Object
{
	char type;
	V3 pix;
	V3 position;
	int radius;
	V3 normal;

} Object;

// Global variables
Object* objects[maxObjects];
int objectCount;

// calls all the following functions and returns a list of pixels
Pixel* raycast(FILE* fp, int width, int height);

// returns the pixel struct of the nearest hit object
V3 shoot(V3 rayVector);
// returns the distance t if the ray vector intersected with the object
double ray_sphere_intersection(V3 rayVector, Object* object);
double ray_plane_intersection(V3 rayVector, Object* object);

int check_file_path(char* fp);

void v3_scale(V3 c, V3 a, double b);
double v3_dot(V3 a, V3 b);
V3 v3_assign(double a, double b, double c);
V3 v3_unit(double a, double b, double c);

#endif
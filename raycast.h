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

#define cameraX 0
#define cameraY 0
#define cameraZ 0

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
V3 r0;
float worldWidth,worldHeight; // these will be the camera width and height

// calls all the following functions and returns a list of pixels
Pixel* raycast(FILE* fp, int width, int height);

// returns the pixel struct of the nearest hit object
V3 shoot(V3 rayVector);
// returns the distance t if the ray vector intersected with the object
double ray_sphere_intersection(V3 rayVector, Object* object);
double ray_plane_intersection(V3 rayVector, Object* object);

int check_file_path(char* fp);

V3 v3_subtract(V3 a, V3 b);
V3 v3_scale(V3 a, double b);
double v3_dot(V3 a, V3 b);
V3 v3_assign(double a, double b, double c);
V3 v3_unit(double a, double b, double c);

void read_file(FILE* fp);
int count_char_in_string(char* inString, char charToCount);

#endif
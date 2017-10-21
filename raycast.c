#include "raycast.h"

int PROPERTY_COLOR = 2;
int PROPERTY_POSITION = 3;
int PROPERTY_NORMAL = 4;

Pixel* raycast(FILE* fp, int width, int height)
{
	// Read in the first line of the file and make sure it is a camera type object
	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

	float cx,cy; // these will be the camera width and height

	// read in the camera line; assumes camera is the first object in the input file
	if((read = getline(&line, &len, fp)) != -1)
	{
		char object_read_in = '\0';
		char* token = strtok(line," ,:\t");	// this string will be chopped up to get the important information about camera
		while(token != NULL)
		{
			if(strcmp(token,"camera") == 0 && object_read_in == '\0') object_read_in = 'c';	// check that camera is the object for this line
			else if(object_read_in != '\0')
			{
				if(strcmp(token,"width") == 0) object_read_in = 'w';	// check if width is the next property for camera
				else if(strcmp(token,"height") == 0) object_read_in = 'h';	// check if the next property is height
				else if(strcmp(token,"0") == 0 || atof(token) > 0)
				{
					if(object_read_in == 'w') cx = atof(token);
					else if(object_read_in == 'h') cy = atof(token);
					else
					{
						fprintf(stderr, "ERROR: Camera property values must be numbers greater than 0\n");
						exit(0);
					}
				}
				else
				{
					fprintf(stderr, "ERROR: Camera properties are incorrect in token: %s\n",token);
					exit(0);
				}
			}
			else
			{
				fprintf(stderr, "ERROR: First object in input file was %s-- SHOULD BE \'camera\'",token);
				exit(0);
			}
			token = strtok(NULL, ",: \t"); // continue breaking up the read in line by commas

		}
		free(token);
	}


	// Store the first object after camera
	int objectsAdded = 0;
	Object* objects = malloc(sizeof(Object)*129);
	while((read = getline(&line, &len, fp)) != -1)
	{
		char object_read_in = '\0';
		char property_read_in = '\0';
		int propertiesAdded = 0;
		int vectorNum;
		if(objectsAdded != 128)
		{
			char* token = strtok(line," ,\t\n:[]");
			while(token != NULL)
			{
				// read in an object
				if(object_read_in == '\0')
				{
					if(strcmp(token,"sphere") == 0)
					{
						objects[objectsAdded].type = 's';
						object_read_in = 's';
					}
					else if(strcmp(token,"plane") == 0)
					{
						objects[objectsAdded].type = 'p';
						object_read_in = 'p';
					}
					else
					{
						fprintf(stderr, "ERROR: Objects can only be type sphere or plan -- NOT %s\n",token);
						exit(0);
					}
				}
				else
				{
					if(strcmp(token,"color") == 0)
					{
						property_read_in = 'c';
						vectorNum = 0;
					}
					else if(strcmp(token,"position") == 0)
					{
						property_read_in = 'p';
						vectorNum = 0;
					}
					else if(strcmp(token,"radius") == 0 && object_read_in == 's')
					{
						property_read_in = 'r';
					}
					else if(strcmp(token,"normal") == 0 && object_read_in == 'p')
					{
						property_read_in = 'n';
						vectorNum = 0;
					}
					else if(property_read_in == 'c')
					{
						if(vectorNum >= 3)
						{
							fprintf(stderr, "ERROR: Pixels can only have 3 channels.\n");
							exit(0);
						}
						else
						{
							// malloc the space to add the pixel if nothing has been stored yet TODO: check if space was malloced???
							if(vectorNum == 0) objects[objectsAdded]->pix = (Pixel*) malloc(sizeof(Pixel));
							// check that the value is valid and store it if it is
							if(strcmp(token,"0") == 0 || atof(token) > 0)
							{
								switch(vectorNum)
								{
									case 0: objects[objectsAdded]->pix->R = atof(token);
									case 1: objects[objectsAdded]->pix->G = atof(token);
									case 2: objects[objectsAdded]->pix->B = atof(token);
									default:
										fprintf(stderr, "ERROR: Pixels only have three channels.\n");
										exit(0);
								}
								vectorNum++;
							}
							else
							{
								fprintf(stderr, "ERROR: Values for the Color Property must be positive numbers-- NOT %s\n", token);
								exit(0);
							}
						}
					}
					else if(property_read_in == 'p')
					{
						if(vectorNum >= 3)
						{
							fprintf(stderr, "ERROR: Position Property can only have 3 coordinates.\n");
							exit(0);
						}
						else
						{
							// malloc the space to add the pixel if nothing has been stored yet TODO: check if space was malloced???
							if(vectorNum == 0) objects[objectsAdded].position = malloc(sizeof(double)*3);
							// check that the value is valid and store it if it is
							if(strcmp(token,"0") == 0 || atof(token) != 0) objects[objectsAdded].position[vectorNum++] = atof(token);
							else
							{
								fprintf(stderr, "ERROR: Values for the Position Property must be numbers-- NOT %s\n", token);
								exit(0);
							}
						}

					}
					else if(property_read_in == 'n')
					{
						if(vectorNum >= 3)
						{
							fprintf(stderr, "ERROR: Normal property can only have 3 coordinates.\n");
							exit(0);
						}
						else
						{
							// malloc the space to add the pixel if nothing has been stored yet TODO: check if space was malloced???
							if(vectorNum == 0) objects[objectsAdded].normal = malloc(sizeof(double)*3);
							// check that the value is valid and store it if it is
							if(strcmp(token,"0") == 0 || atof(token) != 0) objects[objectsAdded].normal[vectorNum++] = atof(token);
							else
							{
								fprintf(stderr, "ERROR: Values for the Normal Property must be numbers-- NOT %s\n", token);
								exit(0);
							}
						}

					}
					else if(property_read_in == 'r')
					{
						if(strcmp(token,"0") == 0 || atoi(token) > 0) objects[objectsAdded].radius = atoi(token);
						else
						{
							fprintf(stderr, "ERROR: Values for the Radius Property must be positive numbers-- NOT %s\n", token);
							exit(0);
						}
					}
					else
					{
						fprintf(stderr, "ERROR: Invalid value or property -- %s\n", token);
						exit(0);
					}
				}
				token = strtok(NULL," ,\t\n:[]");
			}

			if(propertiesAdded != 3)
			{
				fprintf(stderr, "ERROR: Three properties should have been read in -- NOT %d\n", propertiesAdded);
				exit(0);
			}
			objectsAdded++;
		}
		else
		{
			fprintf(stderr, "ERROR: More than 128 objects in input file.\n");
			exit(0);
		}
	}
	if(objectsAdded == 0)
	{
		fprintf(stderr, "ERROR: No objects were read in\n");
		exit(0);
	}

	
	// Finished reading in file, now is the time to start Raycasting!

	Pixel* pixMap = malloc(sizeof(Pixel)*width*height);

	long double pixheight = height / cy; // the height of one pixel
	long double pixwidth = width / cx; // the width of one pixel

	for(int rowCounter = 0; rowCounter < height; rowCounter++)
	{ // for each row
		float py = 0 - cy / 2 + pixheight * (rowCounter + 0.5); // y coord of row

		for(int columnCounter = 0; columnCounter < width; columnCounter++)
		{ // for each column
			float px = 0 - cx / 2 + pixwidth * (columnCounter + 0.5); // x coord of column
			float pz = -1; // z coord is on screen TODO: Is this right?
			V3 ur = v3_unit(px,py,pz); // unit ray vector
			pixMap[rowCounter*width+columnCounter] = shoot(ur, objects,objectsAdded); // return node with the color of what was hit first
		}
    }

	return pixMap;
}

// returns the closest object that intersects with the vector
Pixel* shoot(V3 rayVector, Object* objects, int objectCount)
{
	int hitObjectIndex = -1;
	double t = INFINITY; // no intersection so far
	// loop through the entire linked list of objects and set t to the closest intersected object
	for(int i = 0; i < objectCount; i++ )
	{
		double result;
		// check if the object intersects with the vector
		if(objects[i].type == 's')
		{
			result = ray_sphere_intersection(rayVector,objects[i]);
		}
		else
		{
			result = ray_plane_intersection(rayVector,objects[i]);
		}

		if(result < t)	// this intersection is less than t is already so set t to this result and set hitobject to this object
		{
			t = result;
			hitObjectIndex = i;
		}
	}

	// return the pix of the intersected object
	if(hitObjectIndex != -1)
	{
		return objects[hitObjectIndex]->pix;
	}
	// did not intersect anything, so return a background color pixel
	// return a black pixel, which is the background color
	Pixel* backgroundColor = (Pixel*) malloc(sizeof(Pixel));
	backgroundColor->R = 0;
	backgroundColor->G = 0;
	backgroundColor->B = 0;
	return backgroundColor;
}

// does the math to calculate a sphere intersection, and if the sphere was intersected then the distance to that sphere
double ray_sphere_intersection(V3 rayVector, Object* obj)
{
	double a = v3_dot(rayVector,rayVector);
	V3 positionScaled = malloc(sizeof(double)*3);
	v3_scale(positionScaled,obj->position,-2);
	double b = v3_dot(positionScaled,rayVector);
	double c = v3_dot(obj->position,obj->position)-obj->radius*obj->radius;
	double discriminant = (b*b)-4*a*c;

	// if the discriminant is greater than 0, there was an intersection
	if(discriminant>=0)
	{
		return -b-powf(discriminant,0.5)/(2*a);
	}
	else
	{
		return INFINITY;
	}
}

// does the math for a plane intersection and returns the distance to the plane if the intersection happened
double ray_plane_intersection(V3 rayVector, Object* obj)
{
	float num = v3_dot(obj->normal,obj->position);
	float den = v3_dot(obj->normal, rayVector);
	float t = num/den;
	if(t>0)
	{
		return t;
	}
	else
	{
		return INFINITY;
	}
}

// this function checks if input file exists 
// and returns a 0 if the file exists and a 1 if not
int check_file_path(char* fp)	
{
	FILE *file;
	// if the file can be opened for reading, then it exists
    if ((file = fopen(fp, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

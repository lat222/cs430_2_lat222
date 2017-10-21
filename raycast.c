#include "raycast.h"


Pixel* raycast(FILE* fp, int width, int height)
{
	// Read in the first line of the file and make sure it is a camera type object
	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

	float cw,ch; // these will be the camera width and height

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
					if(object_read_in == 'w') cw = atof(token);
					else if(object_read_in == 'h') ch = atof(token);
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
	objectCount = 0;
	//Object* objects = malloc(sizeof(Object)*129);
	while((read = getline(&line, &len, fp)) != -1)
	{
		char object_read_in = '\0';
		char property_read_in = '\0';
		int propertiesAdded = 0;
		int vectorNum;
		Object* object = (Object*) malloc(sizeof(Object));
		if(objectCount != 128)
		{
			char* token = strtok(line," ,\t\n:[]");
			while(token != NULL)
			{
				// read in an object
				if(object_read_in == '\0')
				{
					if(strcmp(token,"sphere") == 0)
					{
						object->type = 's';
						object_read_in = 's';
					}
					else if(strcmp(token,"plane") == 0)
					{
						object->type = 'p';
						object_read_in = 'p';
					}
					else
					{
						fprintf(stderr, "ERROR: Objects can only be type sphere or plane -- NOT %s\n",token);
						exit(0);
					}
				}
				else
				{
					if(strcmp(token,"color") == 0)
					{
						property_read_in = 'c';
						vectorNum = 0;
						propertiesAdded++;
					}
					else if(strcmp(token,"position") == 0)
					{
						property_read_in = 'p';
						vectorNum = 0;
						propertiesAdded++;
					}
					else if(strcmp(token,"radius") == 0 && object_read_in == 's')
					{
						property_read_in = 'r';
						propertiesAdded++;
					}
					else if(strcmp(token,"normal") == 0 && object_read_in == 'p')
					{
						property_read_in = 'n';
						vectorNum = 0;
						propertiesAdded++;
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
							if(vectorNum == 0) object->pix = malloc(sizeof(double)*3);
							// check that the value is valid and store it if it is
							if(strcmp(token,"0") == 0 || atof(token) > 0) object->pix[vectorNum++] = atof(token);
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
							if(vectorNum == 0) object->position = malloc(sizeof(double)*3);
							// check that the value is valid and store it if it is
							if(strcmp(token,"0") == 0 || atof(token) != 0) object->position[vectorNum++] = atof(token);
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
							if(vectorNum == 0) object->normal = malloc(sizeof(double)*3);
							// check that the value is valid and store it if it is
							if(strcmp(token,"0") == 0 || atof(token) != 0) object->normal[vectorNum++] = atof(token);
							else
							{
								fprintf(stderr, "ERROR: Values for the Normal Property must be numbers-- NOT %s\n", token);
								exit(0);
							}
						}

					}
					else if(property_read_in == 'r')
					{
						if(strcmp(token,"0") == 0 || atoi(token) > 0) object->radius = atoi(token);
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
			objects[objectCount++] = object;
		}
		else
		{
			fprintf(stderr, "ERROR: More than 128 objects in input file.\n");
			exit(0);
		}
	}
	if(objectCount == 0)
	{
		fprintf(stderr, "ERROR: No objects were read in\n");
		exit(0);
	}

	
	// Finished reading in file, now is the time to start Raycasting!

	Pixel* pixMap = (Pixel*) malloc(sizeof(Pixel)*width*height);

	long double pixheight = ch/height; // the height of one pixel
	long double pixwidth = cw/width ; // the width of one pixel

	for(int rowCounter = 0; rowCounter < height; rowCounter++)
	{ // for each row
		float py = -(0 - ch / 2 + pixheight * (rowCounter + 0.5)); // y coord of row

		for(int columnCounter = 0; columnCounter < width; columnCounter++)
		{ // for each column
			float px = 0 - cw / 2 + pixwidth * (columnCounter + 0.5); // x coord of column
			float pz = -1; // z coord is on screen TODO: Is this right?
			V3 ur = v3_unit(px,py,pz); // unit ray vector
			V3 color = shoot(ur);
			pixMap[rowCounter*width+columnCounter].R = color[0]; // return node with the color of what was hit first
			pixMap[rowCounter*width+columnCounter].G = color[1];
			pixMap[rowCounter*width+columnCounter].B = color[2];
		}
    }

	return pixMap;
}

// returns the closest object that intersects with the vector
V3 shoot(V3 rayVector)
{
	int hitObjectIndex = -1;
	double lowest_t = -1; // no intersection so far
	// loop through the entire linked list of objects and set t to the closest intersected object
	for(int i = 0; i < objectCount; i++ )
	{
		double result;
		// check if the object intersects with the vector
		if(objects[i]->type == 's')
		{
			double a = v3_dot(rayVector,rayVector);

			V3 positionScaled = malloc(sizeof(double)*3);
			v3_scale(positionScaled,objects[i]->position,-2);
			double b = v3_dot(positionScaled,rayVector);

			V3 negativePosition = malloc((sizeof(double)*3));
			v3_scale(negativePosition,objects[i]->position,-1);
			double c = v3_dot(negativePosition,negativePosition)-objects[i]->radius*objects[i]->radius;

			double discriminant = (b*b)-4*a*c;

			// if the discriminant is greater than 0, there was an intersection
			if(discriminant > 0)
			{
				double t0 = -b-pow(discriminant,0.5)/(2*a);
				if(t0 > 0) result = t0;
				else
				{
					double t1 = -b+pow(discriminant,0.5)/(2*a);
					if(t1 > 0) result = t1;
					else result = -1;
				}
			}
		}
		else if(objects[i]->type == 'p')
		{
			double num = v3_dot(objects[i]->normal,objects[i]->position);
			double den = v3_dot(objects[i]->normal, rayVector);
			double t = num/den;
			if(t > 0) result = t;
			else result = -1;
		}
		else
		{
			fprintf(stderr, "ERROR: Objects can only be type sphere or plane\n");
			exit(0);
		}

		if(result > 0 && (result < lowest_t || lowest_t == -1))	// this intersection is less than t is already so set t to this result and set hitobject to this object
		{
			//printf("%d - %f\t", i, result);
			lowest_t = result;
			hitObjectIndex = i;
		}
	}

	// return the pix of the intersected object
	if(hitObjectIndex != -1)
	{
		//printf("Hit Object: %d\t", hitObjectIndex);
		return objects[hitObjectIndex]->pix;
	}
	// did not intersect anything, so return a background color pixel
	// return a black pixel, which is the background color
	V3 background = malloc(sizeof(double)*3);
	background[0] = backgroundColorR;
	background[1] = backgroundColorG;
	background[2] = backgroundColorB;
	return background;
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


void v3_scale(V3 c, V3 a, double b)
{
	c[0] = a[0] * b;
	c[1] = a[1] * b;
	c[2] = a[2] * b;
}

double v3_dot(V3 a, V3 b)
{
	double c;
	c = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
	return c;
}

V3 v3_assign(double a, double b, double c)
{
	V3 vector = malloc(sizeof(double) * 3);

	vector[0] = a;
	vector[1] = b;
	vector[2] = c;

	return vector;
}

V3 v3_unit(double a, double b, double c)
{
	V3 vector = malloc(sizeof(double) * 3);

	vector[0] = pow(a/(a*a+b*b+c*c),0.5);
	vector[1] = pow(b/(a*a+b*b+c*c),0.5);
	vector[2] = pow(c/(a*a+b*b+c*c),0.5);

	return vector;
}

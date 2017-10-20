#include "raycast.h"

int PROPERTY_COLOR = 2;
int PROPERTY_POSITION = 3;
int PROPERTY_NORMAL = 4;

node* raycast(FILE* fp, int width, int height)
{
	// create the head of the linked list
	node* headPixel;

	// Read in the first line of the file and make sure it is a camera type object
	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

	float cx,cy; // these will be the camera width and height

	// read in the camera line; assumes camera is the first object in the input file
	if((read = getline(&line, &len, fp)) != -1)
	{
		int object_read_in = 0;
		char* token = strtok(remove_spaces(line),",");	// this string will be chopped up to get the important information about camera
		while(token != NULL)
		{
			if(strcmp(token,"camera") == 0 && object_read_in == 0)	// check that camera is the object for this line
			{
				object_read_in = 1;
			}
			else if(object_read_in != 0)
			{
				if(strncmp(token,"width:",5) == 0)	// check if width is the next property for camera
				{
					cx = atof(cut_string_after_char(token,':'));	// store the width	
				}
				else if(strncmp(token,"height:",7) == 0)	// check if the next property is height
				{
					cy = atof(get_string_after_char(token,':'));	// store the height
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
			token = strtok(NULL, ","); // continue breaking up the read in line by commas

		}
		free(token);
	}


	// Store the first object after camera
	objectNode* headObject;
	if((read = getline(&line, &len, fp)) != -1)
	{
		headObject = readObject(line);
	}
	if(headObject != NULL)
	{
		// then read in and store all of the following objects
		readObjectFile(fp,headObject);
	}

	objectNode* current = headObject;
	while(current != NULL)
	{
		if(current->type == 's') 
			printf("sphere, [%f,%f,%f], [%f,%f,%f], %d\n", current->pix[0],current->pix[1],current->pix[2],current->position[0],current->position[1],current->position[2],current->radius);
		else
			printf("plane, [%f,%f,%f], [%f,%f,%f], [%f,%f,%f]\n", current->pix[0],current->pix[1],current->pix[2],current->position[0],current->position[1],current->position[2],current->normal[0],current->normal[1],current->normal[2]);
	}

	/*
	// Finished reading in file, now is the time to start Raycasting!

	// create a var to tell what pixels we have stored already
	int nodesSaved = 0;

	long double pixheight = height / cy; // the height of one pixel
	long double pixwidth = width / cx; // the width of one pixel

	int rowCounter = 0;
	while(rowCounter < height)
	{ // for each row
		float py = 0 - cy / 2 + pixheight * (rowCounter + 0.5); // y coord of row

		int columnCounter = 0;
		while(columnCounter < width)
		{ // for each column
			float px = 0 - cx / 2 + pixwidth * (columnCounter + 0.5); // x coord of column
			float pz = -1; // z coord is on screen TODO: Is this right?
			vector* ur = make_unit_vector(px,py,pz); // unit ray vector
			node* x = make_node(shoot(ur, headObject)); // return node with the color of what was hit first

			// storing the node as either the head of the linked list or another node in the linked list
			if(nodesSaved==0)
			{
				headPixel = x;
				nodesSaved = 1;
			}
			else
			{
				insert_node(x,headPixel);	// pixel colored by object hit
			}

			columnCounter++;
		}

		rowCounter++;
    }*/

	return headPixel;
}

void readObjectFile(FILE* fp, objectNode* head){
	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    // ONLY the head node exists when this is called, so just create an object and set that to current's next node
    objectNode* current = head;
	while((read = getline(&line, &len, fp)) != -1)
	{
		current->next = readObject(line);
		current = current->next;
		current->next = NULL;
	}
}


objectNode* readObject(char* line)
{
	// reads in an object (sphere or plane ONLY) and assumes that all objects are written in with properties in the exact order as given in the 
	// example for the JSON file for the lab
	objectNode* newObject = (objectNode*) malloc(sizeof(objectNode));
	int object_read_in = 0;

	int vectorItemsStored = 0;

	char* token = strtok(remove_spaces(line),",");
	while(token != NULL)
	{
		if(strcmp(token,"sphere") == 0 && object_read_in == 0)
		{
			newObject->type = 's';
			object_read_in = 1;
		}
		else if(strcmp(token,"plane") == 0 && object_read_in == 0)
		{
			newObject->type = 'p';
			object_read_in = 1;	
		}
		else if(object_read_in != 0)
		{
			if(strncmp(token,"color:",6) == 0)
			{
				char* value = get_string_after_char(token,':');
				newObject->pix = malloc(sizeof(double) * 3);
				newObject->pix[0] = get_first_vector_value(value);
				vectorItemsStored = 1;
				object_read_in = PROPERTY_COLOR;
			}
			else if(strncmp(token,"position:",9) == 0)
			{
				char* value = get_string_after_char(token,':');
				newObject->position = malloc(sizeof(double) * 3);
				newObject->position[0] = get_first_vector_value(value);
				vectorItemsStored = 1;
				object_read_in = PROPERTY_POSITION;
			}
			else if(newObject->type == 's' && strncmp(token,"radius:",7) == 0)
			{
				char* value = get_string_after_char(token,':');
				if(strcmp(value,"0") != 0 && atoi(value) > 0)
				{
					newObject->radius = atoi(value);
				}
				else
				{
					fprintf(stderr, "ERROR: Radius value must be a number greater than 0 -- NOT %s\n", value);
				}
			}
			else if(newObject->type == 'p' && strncmp(token,"normal:",7) == 0)
			{
				char* value = get_string_after_char(token,':');
				newObject->normal = malloc(sizeof(double) * 3);
				newObject->normal[0] = get_first_vector_value(value);
				vectorItemsStored = 1;
				object_read_in = PROPERTY_NORMAL;
			}
			// TODO: vectors will be split into tokens as well-- how to handle????
			else if(vectorItemsStored == 1)
			{
				if(object_read_in == PROPERTY_COLOR)
				{
					if(strcmp(token,"0") == 0 || atof(token) > 0)
					{
						newObject->pix[1] = atof(token);
					}
				}
				else if(object_read_in == PROPERTY_POSITION)
				{
					if(strcmp(token,"0") == 0 || atof(token) > 0)
					{
						newObject->position[1] = atof(token);
					}
				}
				else if(object_read_in == PROPERTY_NORMAL)
				{
					if(strcmp(token,"0") == 0 || atof(token) > 0)
					{
						newObject->normal[1] = atof(token);
					}
				}
				else
				{
					fprintf(stderr,"ERROR: Cannot store 2nd vector number\n");
					exit(0);
				}
				vectorItemsStored = 2;
			}
			else if (vectorItemsStored == 2)
			{
				if(object_read_in == PROPERTY_COLOR)
				{
					newObject->pix[2] = get_last_vector_value(token);
				}
				else if(object_read_in == PROPERTY_POSITION)
				{
					newObject->position[2] = get_last_vector_value(token);
				}
				else if(object_read_in == PROPERTY_NORMAL)
				{
					newObject->normal[2] = get_last_vector_value(token);
				}
				else
				{
					fprintf(stderr,"ERROR: Cannot store 3rd vector number\n");
					exit(0);
				}
				vectorItemsStored = 0;
			}
			else
			{
				fprintf(stderr, "ERROR: Property is invalid in token: %s\n", token);
				exit(0);
			}
		}
		else
		{
			fprintf(stderr, "ERROR: Object %s in input file was not type sphere or plane", token);
			exit(0);
		}
		strtok(NULL,",");
	}
	free(token);

	return newObject;
}

// creates a unit vector
/*vector* make_unit_vector(float x, float y, float z)
{
	float length = powf((powf(x,2.0)+powf(y,2.0)+powf(z,2.0)),0.5);
	vector* unit_vector = (vector*) malloc(sizeof(vector));
	unit_vector->x = x/length;
	unit_vector->y = y/length;
	unit_vector->z = z/length;
	return unit_vector;
}

// returns the closest object that intersects with the vector
pixel* shoot(vector* rayVector, objectNode* head)
{
	objectNode* hitObject;
	objectNode* current = head;
	float t = INFINITY; // no intersection so far
	// loop through the entire linked list of objects and set t to the closest intersected object
	while(current != NULL)
	{
		float result;
		// check if the object intersects with the vector
		if(current->type == 's')
		{
			result = ray_sphere_intersection(rayVector,current);
		}
		else
		{
			result = ray_plane_intersection(rayVector,current);
		}

		if(result < t)	// this intersection is less than t is already so set t to this result and set hitobject to this object
		{
			t = result;
			hitObject = current;
		}
		current = current->next;
	}

	// return the pix of the intersected object
	if(t != INFINITY)
	{
		return hitObject->pix;
	}
	else // did not intersect anything, so return a background color pixel
	{
		// return a black pixel, which is the background color
		pixel* backgroundColor = (pixel*) malloc(sizeof(pixel));
		backgroundColor->R = 0;
		backgroundColor->G = 0;
		backgroundColor->B = 0;
		return backgroundColor;
	}
}

// does the math to calculate a sphere intersection, and if the sphere was intersected then the distance to that sphere
float ray_sphere_intersection(vector* rayVector, objectNode* oNode)
{
	float dx = rayVector->x;
	float dy = rayVector->y;
	float dz = rayVector->z;
	float cx = oNode->position->x;
	float cy = oNode->position->y;
	float cz = oNode->position->z;
	int r = oNode->radius;
	float a = powf(dx,2)+powf(dy,2)+powf(dz,2);
	float b = 2*dx*(0-cx)+2*dy*(0-cy)+2*dz*(0-cz);
	float c = powf(cx,2)+powf(cy,2)+powf(cz,2)-powf(r,2);
	float discriminant = powf(b,2)-4*a*c;

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
float ray_plane_intersection(vector* rayVector, objectNode* oNode)
{
	float num = dot_product(oNode->normal,oNode->position);
	float den = dot_product(oNode->normal, rayVector);
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

// does the dot product of two vectors
float dot_product(vector* v, vector* u)
{
    float result = 0.0;
    result += v->x*u->x;
    result += v->y*u->y;
    result += v->z*u->z;
    return result;
}*/

double get_first_vector_value(char* line)
{
	// check to make sure that the '[' char is in the line
	if(line[0]=='[')
	{
		char* value = get_string_after_char(line,'[');
		// the string value should just be an int. If it is "0", that works. If the atoi of it is greater than 0, it is not a string and it is not negative
		if(strcmp(value,"0") == 0 || atof(value) > 0)
		{
			return(atof(value));
		}
	}
	fprintf(stderr, "ERROR: Values for Color,Position, and Normal properties must be in brackets and positive numbers\n");
	exit(0);
}

double get_last_vector_value(char* line)
{
	// check to make sure that the '[' char is in the line
	if(line[strlen(line)-1]==']')
	{
		char* value = cut_string_at_char(line,']');
		// the string value should just be an int. If it is "0", that works. If the atoi of it is greater than 0, it is not a string and it is not negative
		if(strcmp(value,"0") == 0 || atof(value) > 0)
		{
			return(atof(value));
		}
	}
	fprintf(stderr, "ERROR: Values for Color,Position, and Normal properties must be in brackets and positive numbers\n");
	exit(0);
}
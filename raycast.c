#include "raycast.h"

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
		char* current = malloc(strlen(line) + 1);	// this string will be chopped up to get the important information about camera
		strcpy(current,line);
		if(strcmp(cut_string_at_char(current,','),"camera") == 0)	// check that camera is the object for this line
		{
			current = get_string_after_char(current,' ');	// get to the next property
			if(strcmp(cut_string_at_char(current,':'),"width") == 0)	// check that width is the next property for camera
			{
				cx = atof(cut_string_at_char(get_string_after_char(current,' '),','));	// store the width

				current = get_string_after_char(get_string_after_char(current,','),' ');	// get to the next property
				if(strcmp(cut_string_at_char(current,':'),"height") == 0)	// check the next property is height
				{
					cy = atof(get_string_after_char(current,' '));	// store the height
				}
				else{
					fprintf(stderr, "Camera properties are incorrect\n");
					exit(0);
				}
			}
			else
			{
				fprintf(stderr, "Camera properties are incorrect\n");
				exit(0);
			}
		}
		else
		{
			fprintf(stderr, "First object in input file was %s-- SHOULD BE \'camera\'",cut_string_at_char(current,','));
			exit(0);
		}
		free(current);
	}


	// Store the first object after camera
	objectNode* headObject;
	if((read = getline(&line, &len, fp)) != -1)
	{
		headObject = readObject(line);
	}
	// then read in and store all of the following objects
	readObjectFile(fp,headObject);


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
    }

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
	free(line);
}


objectNode* readObject(char* line)
{
	// reads in an object (sphere or plane ONLY) and assumes that all objects are written in with properties in the exact order as given in the 
	// example for the JSON file for the lab
	objectNode* newObject = (objectNode*) malloc(sizeof(objectNode));
	char* current = line;	
	if(strcmp(cut_string_at_char(current,','),"sphere") == 0)
	{
		newObject->type = 's';
		current = get_string_after_char(current,' ');
		if(strcmp(cut_string_at_char(current,':'),"color") == 0)
		{
			pixel* pix = (pixel*) malloc(sizeof(pixel));
			pix->R = atof(cut_string_at_char(get_string_after_char(current,'['),','));
			pix->G = atof(cut_string_at_char(get_string_after_char(current,','),','));
			pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
			newObject->pix = pix;

			current = get_string_after_char(get_string_after_char(current,']'),' ');
			if(strcmp(cut_string_at_char(current,':'),"position") == 0)
			{
				vector* position = (vector*) malloc(sizeof(vector));
				position->x = atoi(cut_string_at_char(get_string_after_char(current,'['),','));
				position->y = atoi(cut_string_at_char(get_string_after_char(current,','),','));
				position->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
				newObject->position = position;

				current = get_string_after_char(get_string_after_char(current,']'),' ');
				if(strcmp(cut_string_at_char(current,':'),"radius") == 0)
				{
					newObject->radius = atoi(get_string_after_char(current,' '));
				}
				else
				{
					fprintf(stderr, "Sphere property %s is incorrect.\n", cut_string_at_char(current,':'));
					exit(0);
				}
			}
			else
			{
				fprintf(stderr, "Sphere property %s is incorrect.\n", cut_string_at_char(current,':'));
				exit(0);
			}
		}
		else
		{
			fprintf(stderr, "Sphere property %s is incorrect.\n", cut_string_at_char(current,':'));
			exit(0);
		}
	}
	else if(strcmp(cut_string_at_char(current,','),"plane") == 0)
	{
		newObject->type = 'p';
		current = get_string_after_char(current,' ');
		if(strcmp(cut_string_at_char(current,':'),"color") == 0)
		{
			pixel* pix = (pixel*) malloc(sizeof(pixel));
			pix->R = atof(cut_string_at_char(get_string_after_char(current,'['),','));
			pix->G = atof(cut_string_at_char(get_string_after_char(current,','),','));
			pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
			newObject->pix = pix;

			current = get_string_after_char(get_string_after_char(current,']'),' ');
			if(strcmp(cut_string_at_char(current,':'),"position") == 0)
			{
				vector* position = (vector*) malloc(sizeof(vector));
				position->x = atoi(cut_string_at_char(get_string_after_char(current,'['),','));
				position->y = atoi(cut_string_at_char(get_string_after_char(current,','),','));
				position->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
				newObject->position = position;

				current = get_string_after_char(get_string_after_char(current,']'),' ');
				if(strcmp(cut_string_at_char(current,':'),"normal") == 0)
				{
					vector* normal = (vector*) malloc(sizeof(vector));
					normal->x = atoi(cut_string_at_char(get_string_after_char(current,'['),','));
					normal->y = atoi(cut_string_at_char(get_string_after_char(current,','),','));
					normal->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
					newObject->normal = normal;
				}
				else
				{
					fprintf(stderr, "Plane property %s is incorrect.\n", cut_string_at_char(current,':'));
					exit(0);
				}
			}
			else
			{
				fprintf(stderr, "Plane property %s is incorrect.\n", cut_string_at_char(current,':'));
				exit(0);
			}
		}
		else
		{
			fprintf(stderr, "Plane property %s is incorrect.\n", cut_string_at_char(current,':'));
			exit(0);
		}		
	}
	else
	{
		fprintf(stderr, "Object %s in input file was not type sphere or plane", cut_string_at_char(current,','));
		exit(0);
	}

	free(current);

	return newObject;
}

// creates a unit vector
vector* make_unit_vector(float x, float y, float z)
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
}
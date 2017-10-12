#include "raycast.h"

node* raycast(FILE* fp, int width, int height)
{
	// create the head of the linked list
	node* headPixel = (node*) malloc(sizeof(node));

	// Read in the first line of the file and make sure it is a camera type object
	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

	float cx,cy;

	if((read = getline(&line, &len, fp)) != -1)
	{
		char* current = malloc(strlen(line) + 1);
		strcpy(current,line);
		if(strcmp(cut_string_at_char(current,','),"camera") == 0)
		{
			current = get_string_after_char(current,' ');
			if(strcmp(cut_string_at_char(current,':'),"width") == 0)
			{
				cx = atof(cut_string_at_char(get_string_after_char(current,' '),','));

				current = get_string_after_char(get_string_after_char(current,','),' ');
				if(strcmp(cut_string_at_char(current,':'),"height") == 0)
				{
					cy = atof(get_string_after_char(current,' '));
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

	objectNode* headObject;
	if((read = getline(&line, &len, fp)) != -1)
	{
		headObject = readObject(line);
	}
	readObjectFile(fp,headObject);


	// Finished reading in file, now is the time to start Raycasting!
	// M and N are arbitrary amounts of pixels
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
			objectNode* x = shoot(ur, headObject); // return position of first hit
			insert_node(make_node(shade(x)),headPixel);	// pixel colored by object hit; TODO: fix what make_node takes in
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

    objectNode* current = head;
	while((read = getline(&line, &len, fp)) != -1)
	{
		if(current->next == NULL)
		{
			current->next = readObject(line);
		}
		current = current->next;
		current->next = NULL;
	}
	free(line);
}


objectNode* readObject(char* line)
{
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

vector* make_unit_vector(float x, float y, float z)
{
	float length = powf((powf(x,2.0)+powf(y,2.0)+powf(z,2.0)),0.5);
	vector* unit_vector = (vector*) malloc(sizeof(vector));
	unit_vector->x = x/length;
	unit_vector->y = y/length;
	unit_vector->z = z/length;
	return unit_vector;
}

objectNode* shoot(vector* rayVector, objectNode* head)
{
	return head;
}

pixel* shade(objectNode* hitObject)
{
	return hitObject->pix;
}
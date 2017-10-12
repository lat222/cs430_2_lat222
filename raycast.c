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

	int cx,cy;

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
				printf("cx = %d\n", cx);
				current = get_string_after_char(get_string_after_char(current,','),' ');
				// Causes a SEG Fault!!! FUCK!!!
				/*if(strcmp(cut_string_at_char(current,':'),"height") == 0)
				{
					cy = atof(cut_string_at_char(get_string_after_char(current,' '),','));
					printf("cy = %d\n", cy);
				}
				else{
					fprintf(stderr, "Camera properties are incorrect\n");
					exit(0);
				}*/
			}
			else if(strcmp(cut_string_at_char(current,':'),"height") == 0)
			{
				cy = atof(cut_string_at_char(get_string_after_char(current,' '),','));
				current = get_string_after_char(get_string_after_char(current,','),' ');
				if(strcmp(cut_string_at_char(current,':'),"width") == 0)
				{
					cx = atof(cut_string_at_char(get_string_after_char(current,' '),','));
				}
				else
				{
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

	objectNode* headObject = readObjectFile(fp);

	/*float pixheight = float(h) / float(M); // the height of one pixel
	float pixwidth = float(w) / float(N); // the width of one pixel

	int rowCounter = 0;
	while(rowCounter < M)
	{ // for each row
		int py = cy - h / 2 + pixheight * (rowCounter + 0.5); // y coord of row
		int columnCounter = 0;

		while(columnCounter < N)
		{ // for each column
			int px = cx - w / 2 + pixwidth * (columnCounter + 0.5); // x coord of column
			int pz = −zp; // z coord is on screen
			vector* ur = p/kpk; // unit ray vector
			x = shoot(ur, headObject); // return position of first hit
			insert_node(make_node(shade(x)),head);	// pixel colored by object hit; TODO: fix what make_node takes in
		}

		rowCounter++;
    }*/

	return headPixel;
}

objectNode* readObjectFile(FILE* fp){
	// reads in all the objects and stores them in a linked list
	objectNode* headObject = (objectNode*) malloc(sizeof(objectNode));

	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    objectNode* current;
	while((read = getline(&line, &len, fp)) != -1)
	{
		/*if(headObject == NULL)
		{
			headObject = readObject(line);
			current = headObject;
		}
		else
		{
			current = readObject(line);
		}
		current->next = NULL;
		current = current->next;*/
		current = readObject(line);
		printf("%c, %d, %d, %d \n",current->type, current->pix->R, current->position->x, current->radius);

	}

	// TODO: Delete this shit
	/*current = headObject;
	while(current != NULL)
	{
		printf("%c, %d, %d, %d \n",current->type, current->pix->R, current->position->x, current->radius);
		current = current->next;
	}*/

	return headObject;

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
			else if(strcmp(cut_string_at_char(current,':'),"radius") == 0)
			{
				newObject->radius = atoi(cut_string_at_char(get_string_after_char(current,' '),','));

				current = get_string_after_char(get_string_after_char(current,','),' ');
				if(strcmp(cut_string_at_char(current,':'),"position") == 0)
				{
					vector* position = (vector*) malloc(sizeof(vector));
					position->x = atoi(cut_string_at_char(get_string_after_char(current,'['),','));
					position->y = atoi(cut_string_at_char(get_string_after_char(current,','),','));
					position->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
					newObject->position = position;
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
		else if(strcmp(cut_string_at_char(current,':'),"position") == 0)
		{
			vector* position = (vector*) malloc(sizeof(vector));
			position->x = atoi(cut_string_at_char(get_string_after_char(current,'['),','));
			position->y = atoi(cut_string_at_char(get_string_after_char(current,','),','));
			position->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
			newObject->position = position;

			current = get_string_after_char(get_string_after_char(current,','),' ');
			if(strcmp(cut_string_at_char(current,':'),"radius") == 0)
			{
				newObject->radius = atoi(cut_string_at_char(get_string_after_char(current,' '),','));

				current = get_string_after_char(get_string_after_char(current,','),' ');
				if(strcmp(cut_string_at_char(current,':'),"color") == 0)
				{
					pixel* pix = (pixel*) malloc(sizeof(pixel));
					pix->R = atof(cut_string_at_char(get_string_after_char(current,'['),','));
					pix->G = atof(cut_string_at_char(get_string_after_char(current,','),','));
					pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
					newObject->pix = pix;
				}
				else
				{
					fprintf(stderr, "Sphere property %s is incorrect.\n", cut_string_at_char(current,':'));
					exit(0);
				}
			}
			else if(strcmp(cut_string_at_char(current,':'),"color") == 0)
			{
				pixel* pix = (pixel*) malloc(sizeof(pixel));
				pix->R = atof(cut_string_at_char(get_string_after_char(current,'['),','));
				pix->G = atof(cut_string_at_char(get_string_after_char(current,','),','));
				pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
				newObject->pix = pix;

				current = get_string_after_char(get_string_after_char(current,','),' ');
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
		else if(strcmp(cut_string_at_char(current,':'),"radius") == 0)
		{
			newObject->radius = atoi(cut_string_at_char(get_string_after_char(current,' '),','));

			current = get_string_after_char(get_string_after_char(current,','),' ');
			if(strcmp(cut_string_at_char(current,':'),"color") == 0)
			{
				pixel* pix = (pixel*) malloc(sizeof(pixel));
				pix->R = atof(cut_string_at_char(get_string_after_char(current,'['),','));
				pix->G = atof(cut_string_at_char(get_string_after_char(current,','),','));
				pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
				newObject->pix = pix;

				current = get_string_after_char(get_string_after_char(current,','),' ');
				if(strcmp(cut_string_at_char(current,':'),"position") == 0)
				{
					vector* position = (vector*) malloc(sizeof(vector));
					position->x = atoi(cut_string_at_char(get_string_after_char(current,'['),','));
					position->y = atoi(cut_string_at_char(get_string_after_char(current,','),','));
					position->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
					newObject->position = position;
				}
				else
				{
					fprintf(stderr, "Sphere property %s is incorrect.\n", cut_string_at_char(current,':'));
					exit(0);
				}
			}
			else if(strcmp(cut_string_at_char(current,':'),"position") == 0)
			{
				vector* position = (vector*) malloc(sizeof(vector));
				position->x = atoi(cut_string_at_char(get_string_after_char(current,'['),','));
				position->y = atoi(cut_string_at_char(get_string_after_char(current,','),','));
				position->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
				newObject->position = position;

				current = get_string_after_char(get_string_after_char(current,','),' ');
				if(strcmp(cut_string_at_char(current,':'),"color") == 0)
				{
					pixel* pix = (pixel*) malloc(sizeof(pixel));
					pix->R = atof(cut_string_at_char(get_string_after_char(current,'['),','));
					pix->G = atof(cut_string_at_char(get_string_after_char(current,','),','));
					pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,','),','),']'));
					newObject->pix = pix;
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
	}
	else
	{
		fprintf(stderr, "Object %s in input file was not type sphere or plane", cut_string_at_char(current,','));
		exit(0);
	}

	return newObject;
}

objectNode* shoot(vector* rayVector, objectNode* head)
{
	return head;
}

pixel* shade(objectNode* hitObject)
{
	return hitObject->pix;
}
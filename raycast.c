#include "raycast.h"

node* raycast(FILE* fp, width, height)
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
		char* current = read;
		if(strcmp(cut_string_at_char(current,","),"camera") == 1)
		{
			current = get_string_after_char(current," ");
			if(strcmp(cut_string_at_char(current,":"),"width") == 1)
			{
				cx = atoi(cut_string_at_char(get_string_after_char(current," "),","));
				current = get_string_after_char(get_string_after_char(current," "), " ");
				if(strcmp(cut_string_at_char(current,":"),"height") == 1)
				{
					cy = atoi(cut_string_at_char(get_string_after_char(current," "),","));
				}
				else{
					fprintf(stderr, "Camera properties are incorrect\n");
					exit();
				}
			}
			else if(strcmp(cut_string_at_char(current,":"),"height") == 1)
			{
				cy = atoi(cut_string_at_char(get_string_after_char(current," "),","));
				current = get_string_after_char(get_string_after_char(current," "), " ");
				if(strcmp(cut_string_at_char(current,":"),"width") == 1)
				{
					cx = atoi(cut_string_at_char(get_string_after_char(current," "),","));
				}
				else
				{
					fprintf(stderr, "Camera properties are incorrect\n");
					exit();
				}
			}
			else
			{
				fprintf(stderr, "Camera properties are incorrect\n");
				exit();
			}
		}
		else
		{
			fprintf(stderr, "First object in input file was not \'camera\'");
			exit();
		}
	}

	objectNode* headObject = readObjects(fp);

	float pixheight = float(h) / float(M); // the height of one pixel
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
    }

	return headPixel;
}

objectNode* readObjectFile(FILE* fp){
	// reads in all the objects and stores them in a linked list
	objectNode* headObject;

	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    objectNode* current;
	while((read = getline(&line, &len, fp)) != -1)
	{
		if(headObject == NULL)
		{
			headObject = readObject(line);
			headObject->nextObjectNode = NULL;
			current = headObject;

		}
		else
		{
			current = readObject(line);
			current->nextObjectNode = NULL;
		}
		current = current->nextObjectNode;
	}

	return headObject;

}

objectNode* readObject(char* line)
{
	objectNode* newObject = (objectNode*) malloc(sizeof(objectNode));
	char* current = line;
	int properties = 0;	
	if(strcmp(cut_string_at_char(current,","),"sphere") == 1)
	{
		newObject->type = 's';
		newObject->normal = NULL;
		current = get_string_after_char(current," ");
		if(strcmp(cut_string_at_char(current,":"),"color") == 1)
		{
			pixel* pix = (pixel*) malloc(sizeof(pixel));
			pix->R = atof(cut_string_at_char(get_string_after_char(current,"["),","));
			pix->G = atof(cut_string_at_char(get_string_after_char(current,","),","));
			pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
			newObject->pix = pix;

			current = get_string_after_char(get_string_after_char(current," "), " ");
			if(strcmp(cut_string_at_char(current,":"),"postion") == 1)
			{
				vector* position = (vector*) malloc(sizeof(vector));
				postion->x = atoi(cut_string_at_char(get_string_after_char(current,"["),","));
				postion->y = atoi(cut_string_at_char(get_string_after_char(current,","),","));
				postion->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
				newObject->position = position;

				current = get_string_after_char(get_string_after_char(current," "), " ");
				if(strcmp(cut_string_at_char(current,":"),"radius") == 1)
				{
					newObject->radius = atoi(get_string_after_char(current," "));
				}
				else
				{
					fprintf(stderr, "Sphere properties are incorrect\n");
					exit();
				}
			}
			else if(strcmp(cut_string_at_char(current,":"),"radius") == 1)
			{
				newObject->radius = atoi(cut_string_at_char(get_string_after_char(current," "),","));

				current = get_string_after_char(get_string_after_char(current," "), " ");
				if(strcmp(cut_string_at_char(current,":"),"postion") == 1)
				{
					vector* position = (vector*) malloc(sizeof(vector));
					postion->x = atoi(cut_string_at_char(get_string_after_char(current,"["),","));
					postion->y = atoi(cut_string_at_char(get_string_after_char(current,","),","));
					postion->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
					newObject->position = position;
				}
				else
				{
					fprintf(stderr, "Sphere properties are incorrect\n");
					exit();
				}
			}
			else
			{
				fprintf(stderr, "Sphere properties are incorrect\n");
				exit();
			}
		}
		else if(strcmp(cut_string_at_char(current,":"),"position") == 1)
		{
			vector* position = (vector*) malloc(sizeof(vector));
			postion->x = atoi(cut_string_at_char(get_string_after_char(current,"["),","));
			postion->y = atoi(cut_string_at_char(get_string_after_char(current,","),","));
			postion->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
			newObject->position = position;

			current = get_string_after_char(get_string_after_char(current," "), " ");
			if(strcmp(cut_string_at_char(current,":"),"radius") == 1)
			{
				newObject->radius = atoi(cut_string_at_char(get_string_after_char(current," "),","));

				current = get_string_after_char(get_string_after_char(current," "), " ");
				if(strcmp(cut_string_at_char(current,":"),"color") == 1)
				{
					pixel* pix = (pixel*) malloc(sizeof(pixel));
					pix->R = atof(cut_string_at_char(get_string_after_char(current,"["),","));
					pix->G = atof(cut_string_at_char(get_string_after_char(current,","),","));
					pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
					newObject->pix = pix;
				}
				else
				{
					fprintf(stderr, "Sphere properties are incorrect\n");
					exit()
				}
			}
			else if(strcmp(cut_string_at_char(current,":"),"color") == 1)
			{
				pixel* pix = (pixel*) malloc(sizeof(pixel));
				pix->R = atof(cut_string_at_char(get_string_after_char(current,"["),","));
				pix->G = atof(cut_string_at_char(get_string_after_char(current,","),","));
				pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
				newObject->pix = pix;

				current = get_string_after_char(get_string_after_char(current," "), " ");
				if(strcmp(cut_string_at_char(current,":"),"radius") == 1)
				{
					newObject->radius = atoi(get_string_after_char(current," "));
				}
				else
				{
					fprintf(stderr, "Sphere properties are incorrect\n");
					exit();
				}
			}
			else
			{
				fprintf(stderr, "Sphere properties are incorrect\n");
				exit();
			}
		}
		else if(strcmp(cut_string_at_char(current,":"),"radius") == 1)
		{
			newObject->radius = atoi(cut_string_at_char(get_string_after_char(current," "),","));

			current = get_string_after_char(get_string_after_char(current," "), " ");
			if(strcmp(cut_string_at_char(current,":"),"color") == 1)
			{
				pixel* pix = (pixel*) malloc(sizeof(pixel));
				pix->R = atof(cut_string_at_char(get_string_after_char(current,"["),","));
				pix->G = atof(cut_string_at_char(get_string_after_char(current,","),","));
				pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
				newObject->pix = pix;

				current = get_string_after_char(get_string_after_char(current," "), " ");
				if(strcmp(cut_string_at_char(current,":"),"position") == 1)
				{
					vector* position = (vector*) malloc(sizeof(vector));
					postion->x = atoi(cut_string_at_char(get_string_after_char(current,"["),","));
					postion->y = atoi(cut_string_at_char(get_string_after_char(current,","),","));
					postion->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
					newObject->position = position
				}
				else
				{
					fprintf(stderr, "Sphere properties are incorrect\n");
					exit();
				}
		}
		else if(strcmp(cut_string_at_char(current,":"),"position") == 1)
		{
			vector* position = (vector*) malloc(sizeof(vector));
			postion->x = atoi(cut_string_at_char(get_string_after_char(current,"["),","));
			postion->y = atoi(cut_string_at_char(get_string_after_char(current,","),","));
			postion->z = atoi(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
			newObject->position = position;

			current = get_string_after_char(get_string_after_char(current," "), " ");
			if(strcmp(cut_string_at_char(current,":"),"color") == 1)
			{
				pixel* pix = (pixel*) malloc(sizeof(pixel));
				pix->R = atof(cut_string_at_char(get_string_after_char(current,"["),","));
				pix->G = atof(cut_string_at_char(get_string_after_char(current,","),","));
				pix->B = atof(cut_string_at_char(get_string_after_char(get_string_after_char(current,","),","),"]"));
				newObject->pix = pix;
			}
			else
			{
				fprintf(stderr, "Sphere properties are incorrect\n");
				exit();
			}

		}
		else
		{
			fprintf(stderr, "Sphere properties are incorrect\n");
			exit();
		}
	}
	else if(strcmp(cut_string_at_char(current,","),"plane") == 1)
	{
		newObject->type = 'p';
		newObject->radius = NULL;
	}
	else
	{
		fprintf(stderr, "Object %s in input file was not type sphere or plane", cut_string_at_char(current,","));
		exit();
	}

	return newObject;
}

object* shoot(vector* rayVector, objectNode* head)
{

}

pixel* shade(object* hitObject)
{
	return hitObject->pix;
}
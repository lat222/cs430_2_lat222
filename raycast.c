#include "raycast.h"

node* raycast(FILE* fp, width, height)
{
	// create the head of the linked list
	node* headPixel = (node*) malloc(sizeof(node));

	// Assume the first line is always the camera
	// then set camera width and height to cx and cy
	char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if ((read = getline(&line, &len, fp)) != -1)
    {
    	// check that the first object is a camera
    	char* token = strtok(line,",")
    	if(strcmp(token,"camera") == 1)
    	{
    		int cx,cy;
    		token = strtok(NULL,",");
    		while(token)
    		{
    			
    			token = strtok(NULL,",");
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
    	}
    	else
    	{
    		fprintf(stderr,"ERROR: First object in input file was not 'camera'");
    	}
    }

	return headPixel;
}

objectNode* readObjects(FILE* fp){
	// reads in all the objects and stores them in a linked list
	objectNode* headObject = objectNode* malloc(sizeof(objectNode));

	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    current  = headObject;
	while((read = getline(&line, &len, fp)) != -1)
	{
		// TODO: Decide whether I need to malloc the space for the new node???
		current->type = "";
		current->nextObjectNode = NULL;
		current = current->nextObjectNode;
	}

	return headObject;

}

object* shoot(vector* rayVector, objectNode* head)
{

}

pixel* shade(object* hitObject)
{
	return hitObject->pix;
}
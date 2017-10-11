#include "raycast.h"

node* raycast(file* fp, width, height)
{
	// create the head of the linked list
	node* head = (node*) malloc(sizeof(node));

	float pixheight = float(h) / float(M); // the height of one pixel
	float pixwidth = float(w) / float(N); // the width of one pixel
	int rowCounter = 0;
	while(rowCounter < M){ // for each row
		int py = cy - h / 2 + pixheight * (rowCounter + 0.5); // y coord of row
		int columnCounter = 0;
		while(columnCounter < N){ // for each column
			int px = cx - w / 2 + pixwidth * (columnCounter + 0.5); // x coord of column
			int pz = −zp; // z coord is on screen
			vector* ur = p/kpk; // unit ray vector
			x = shoot(ur); // return position of first hit
			insert_node(make_node(shade(x)),head);	// pixel colored by object hit; TODO: fix what make_node takes in
		}
		rowCounter++;
	}
}

object* shoot(vector* rayVector)
{

}

pixel* shade(object* hitObject)
{
	return hitObject->pix;
}
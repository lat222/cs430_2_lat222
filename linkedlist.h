#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdio.h>
#include <stdlib.h>

typedef struct pixel {

	// The is where the colors for a pixel are stored
    float R, G, B;

} pixel;

/* A linked list node */
typedef struct node
{
    pixel* pix;
    struct node* next;

}node;

// makes a node based on args and returns the new node
node* make_node(pixel* pix);

// inserts a node into the end of the linked list
void insert_node(node *newNode, node *head);

#endif
#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdio.h>
#include <stdlib.h>

typedef struct pixel {

	// The is where the colors for a pixel are stored
    unsigned char R, G, B;

} pixel;

/* A linked list node */
typedef struct node
{
    pixel pix;
    struct node *next;
    
}node;

// makes a node based on args and returns the new node
node* make_node(unsigned char R, unsigned char G, unsigned char B);

// inserts a node into the end of the linked list
void insert_node(node *newNode, node *head);


#endif
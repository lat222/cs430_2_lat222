#include "linkedlist.h"
 
node* make_node(pixel* pix)
{
    // allocate some space for the node
    node* newNode = (node*) malloc(sizeof(node));

    // check the space was allocated
    if(newNode == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(-1);
    }

    // set newNode's pix to parameter pix;
    newNode->pix = pix;

    // don't connect the node to anything
    newNode->next = NULL;

    // return the newly created node
    return newNode;
}

// editted, but based off of 
// http://stackoverflow.com/questions/5797548/c-linked-list-inserting-node-at-the-end
 void insert_command(node *newNode, node *head){

    // set current to the head of the linked list
    // current will then be used to loop through the entire list until the end, where the newNode will be added
    node *current = head;
    while(1) 
    {
        if(current->next == NULL)
        {
            current->next = newNode;
            break;
        }
        current = current->next;
    }
}
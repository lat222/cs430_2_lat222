#include "ppmformatter.h"


// write to file in p3 format
void write_p3(FILE* fp, node* head)
{
	// set current to the head of the linked list
    // current will then be used to loop through the entire list until the end, where the last node's next is NULL
    // for all the nodes in between the pixel will be written into the file.
    node *current = head;
    while(1) 
    {
        if(current->next == NULL)
        {
            break;
        }
        fprintf(fp, "%d\n%d\n%d\n", current->pix->R, current->pix->G, current->pix->B);
        current = current->next;
    }
}
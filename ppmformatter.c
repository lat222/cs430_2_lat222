#include "ppmformatter.h"

int maxColor = 255;

// write to file in p3 format
void write_p3(FILE* fp, node* head, int width, int height)
{
    // write in the header
    //fprintf(fp, "P3\n%d %d\n%d\n", width, height,maxColor);
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
        fprintf(fp, "%d %d %d\n", (int) current->pix->R*maxColor, (int) current->pix->G*maxColor, (int) current->pix->B*maxColor);
        current = current->next;
    }
}
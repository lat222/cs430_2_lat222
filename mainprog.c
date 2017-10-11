#include "linkedlist.h"
#include "ppmformatter.h"
#include "raycast.h"
#include "helperfuncs.h"

int main(int argc, char* argv[]) 
{
	// checks that the correct number of command line arguments were given
	if (argc != 5)		// any number other than 4 arguments means the wrong amount given
	{
		fprintf(stderr, "Error: Incorrect number of arguments. Requires 4 arguments-- NOT %d.\n", argc-1);
		return 1;
	}
	else	// the right amount of command line arguments(4) were given
	{
		// check that the input file exists
		if (check_file_path(argv[3]) == 1)
		{
			// create the file pointer and open the input file for reading
			FILE* fp= fopen(argv[3], "r");
			node* head  = raycast(fp,argv[1],argv[2]); //TODO: define this function and decide if height and width matter...
		    fclose(fp);		// done with the input file so close it
			
		    // WRITING BEGINS
			fp = fopen(argv[4], "w");	// open the file to write to
			// write header into file
			fprintf(fp, "P3\n%d %d\n%d\n", width, height, maxColor); //TODO: how are these numbers decided????????

			write_p3(fp, head);

			fclose(fp);		// done with the output file so close it

		}
		// input file does not exist
		else
		{
			fprintf(stderr, "Error: File with name %s does not exist.\n", argv[3]);
			return 1;
		}

	}
		
	return 0;		// success so return 0
}
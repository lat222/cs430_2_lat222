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
			if((strcmp(argv[1],"0") != 0 && atoi(argv[1]) > 0) &&
				(strcmp(argv[2],"0") != 0 && atoi(argv[2]) > 0))
			{
				node* head  = raycast(fp,atoi(argv[1]),atoi(argv[2]));
			    fclose(fp);		// done with the input file so close it
				
			    // WRITING BEGINS
				fp = fopen(argv[4], "w");	// open the file to write to

				write_p3(fp, head, atoi(argv[1]), atoi(argv[2]));

				fclose(fp);		// done with the output file so close it
			}
			else
				fprintf(stderr, "Width (%s) or/and Height (%s) argument is not a number greater than 0\n",argv[1],argv[2]);
				return 1;

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
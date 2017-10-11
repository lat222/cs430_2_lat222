#include "ppmformatter.h"

// below are global variables that will be accessible throughout the entire program
char* ppmFormat;
int width;
int height;
int maxColor;
pixel* pixMap;

// this function checks if input file exists 
// and returns a 0 if the file exists and a 1 if not
int check_file_path(char* fp)	
{
	FILE *file;
	// if the file can be opened for reading, then it exists
    if ((file = fopen(fp, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

// function to read in header (as was described in Chapter 2) from input file
// header information is stored to global variables above
int read_header(FILE* fp)
{
	// variables to store what is read in from input file
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    // readHeader marks that none of the header has been stored yet.
    int readHeader = 0;

    // loop to read in all pertinent header information (i.e. ppm format, width, height, and maxColor of the image)
    while (readHeader < 4) {

    	// check that a line has been read in
    	if((read = getline(&line, &len, fp)) != -1)
    	{
    		// check that the line is not a comment
    		if(line[0] != '#')
    		{
    			char* token = strtok(line, " \t");	// split the line on the first whitespace

    			// check to make sure the new token could be split on the whitespace
    			while(token)
    			{
    				// based on what has already been stored, switch to store new header information
    				switch(readHeader)
		    		{
		    			case 0:
		    				ppmFormat = (char*) malloc(sizeof(char)*2); // create the space to store this string
		    				strncpy(ppmFormat,token,2);		// copy the token to the global variable ppmFormat
		    				readHeader++;
		    				break;

		    			case 1:
		    				width = atoi(token);
		    				readHeader++;
		    				break;

		    			case 2:
		    				height = atoi(token);
		    				readHeader++;
		    				break;

		    			case 3:
		    				maxColor = atoi(token);
		    				// check that maxColor is in acceptable range
		    				if (maxColor > 255 || maxColor < 0)
		    				{
		    					fprintf(stderr, "Error: Input file's max color should range from  0-255 -- NOT %d.\n", maxColor);
		    					return 1;
		    				}
		    				readHeader++;
		    				break;

		    		}
		    		token = strtok(NULL, " \t"); // continue breaking up the read in line by white space
    			}    			
    		}
    	}
    	// no line has been read in; so the file and the header are wrong
    	else
    	{
    		fprintf(stderr, "Error: Header is incorrect.\n");
    		return 1;	
    	}
    }

    // free up the space used by line
	if (line)
    {
        free(line);
    }

    return 0;	// successful so return 0

}

// read in a ppm with p3 format
int read_p3(FILE* fp)
{
	int r,g,b;		// the ints that will store each pixel's RGB values
	int pixelsSaved = 0;	// stores the number of pixels already in memory

	// loop through the entire image of pixels from the input file as defined by the file's height and width
	while(pixelsSaved < width*height)
	{
		// save the incoming pixel as long as it exists
		if(fscanf(fp,"%d", &r) != EOF && fscanf(fp,"%d", &g) != EOF && fscanf(fp,"%d", &b) != EOF)
		{
			// make sure each color of the pixel is less than or equal to the maxColor
			if(r > maxColor || g > maxColor || b > maxColor)
			{
				fprintf(stderr, "Error: R, G, or B of pixel %d is not within max color range.\n", pixelsSaved);
				return 1;
			}
			// otherwise, store the pixel values within pixMap array of pixel structs
			pixMap[pixelsSaved].R = r;
			pixMap[pixelsSaved].G = g;
			pixMap[pixelsSaved].B = b;
			pixelsSaved++;		// increase the index for pixels stored
		}
		// all of the pixels do not exist; so err out
		else
		{
			fprintf(stderr, "Error: End of file reached before all pixels read.\n");
			return 1;
		}
	}
	return 0;	// success so return 0
}

// read in a ppm with p6 format
int read_p6(FILE* fp)
{
	unsigned char rawPixel[3];	// array to store the each pixel's RGB values

	int pixelsSaved = 0;	// index for how many pixels have been stored

	// loop through the entire image of pixels from the input file as defined by the file's height and width
	while(pixelsSaved < width*height)
	{
		// read in the 3 byte chunk that give the R,G,B values for the pixel
		if(fread(rawPixel, sizeof(unsigned char), 3, fp) == 3)
		{
			// store the pixel values within pixMap array of pixel structs
			pixMap[pixelsSaved].R = rawPixel[0];
			pixMap[pixelsSaved].G = rawPixel[1];
			pixMap[pixelsSaved].B = rawPixel[2];
			pixelsSaved++;		// increase the index for pixels stored
		}
		else
		{
			fprintf(stderr, "Error: Pixel %d did not read in 3 bytes.\n", pixelsSaved);
			return 1;
		}
	}
	return 0;	// success so return 0
}

// write to file in p3 format
void write_p3(FILE* fp)
{
	int pixelsWritten = 0;

	// loop through all the stored pixels and write their values to the output file
	while (pixelsWritten < height*width)
	{
		fprintf(fp, "%d\n%d\n%d\n", pixMap[pixelsWritten].R, pixMap[pixelsWritten].G, pixMap[pixelsWritten].B);
		pixelsWritten++;
	}
}

// write to file in p6 format
void write_p6(FILE* fp)
{
	// writes everyhting stored in pixmap to the output file
	fwrite(pixMap, sizeof(pixel), height*width, fp);
}
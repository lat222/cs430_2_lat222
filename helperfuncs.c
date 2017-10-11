#include "helperfuncs.h"

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

char* cut_string_at_char(char* inString, char charToCutOut)
{
	// cuts inString at character charToCutOut (exclusive)
	char* returnString  = (char*) malloc((strlen(inString)+1)*sizeof(char)); 
	int pointer = 0;
	while(inString[pointer] != charToCutOut)
	{
		returnString[pointer] = inString[pointer];
		pointer++;
	}
	returnString[pointer] = '\0';
	return returnString;
}

char* cut_string_after_char(char* inString, char charToCutOut)
{
	// cuts inString string after character charToCutOut (inclusive)
	char* returnString  = (char*) malloc((strlen(inString)+1)*sizeof(char)); 
	int pointer = 0;
	while(inString[pointer] != charToCutOut)
	{
		returnString[pointer] = inString[pointer];
		pointer++;
	}
	returnString[pointer++] = charToCutOut;
	returnString[pointer] = '\0';
	return returnString;
}

char* get_string_after_char(char* inString, char charToCutOut)
{
	// cuts inString String after charToCutOut character and gives the half after charToCutOut (exclusive)
	char* returnString  = (char*) malloc((strlen(inString)+1)*sizeof(char)); 
	int inStringPointer = 0;
	int pointer = 0;
	while(inString[inStringPointer] != charToCutOut)
	{
		inStringPointer++;
	}
	inStringPointer++;
	while(inString[inStringPointer] != '\0')
	{
		returnString[pointer++] = inString[inStringPointer++];
	}
	returnString[pointer] = '\0';
	return returnString;
}
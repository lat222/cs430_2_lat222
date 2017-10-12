#ifndef HELPERFUNCS
#define HELPERFUNCS

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int check_file_path(char* fp);

// exclusive
char* cut_string_at_char(char* inString, char charToCutOut); // cuts inString at character charToCutOut

// inclusive
char* cut_string_after_char(char* inString, char charToCutOut); // cuts inString string after character charToCutOut

// exclusive
char* get_string_after_char(char* inString, char charToCutOut); // cuts inString String after charToCutOut character and gives the half after charToCutOut

int count_char_in_string(char* inString, char charToCount);

#endif
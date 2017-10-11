#ifndef HELPERFUNCS
#define HELPERFUNCS

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* cut_string_at_char(char* inString, char charToCutOut); // cuts inString at character charToCutOut (exclusive)

char* cut_string_after_char(char* inString, char charToCutOut); // cuts inString string after character charToCutOut (inclusive)

char* get_string_after_char(char* inString, char charToCutOut); // cuts inString String after charToCutOut character and gives the half after charToCutOut (exclusive)

#endif
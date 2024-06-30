#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>

void clearConsole();
void pause();
int menu(int (*show_options)());
int showOptions(const char** options, int options_count);
void validateOption(int* option, int min, int max, char* error_message);
void readWord(char str[], size_t size);
#endif
#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

void clearConsole();
void pause();
int menu(int (*show_options)());
int showOptions(const char** options, int options_count);
void validateOption(int* option, int min, int max, char* error_message);
bool inRange(int value, int min, int max);
#endif
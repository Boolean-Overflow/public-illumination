#ifndef __UTILS_H__
#define __UTILS_H__

void clear_console();
void pause(char* str);
int menu(int (*show_options)());
int show_options(const char** options, int options_count);
void validate_option(int* option, int min, int max, char* error_message);

#endif
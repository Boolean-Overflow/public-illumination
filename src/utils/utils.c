#include "./utils.h"

#include <stdio.h>
#include <stdlib.h>

void clear_console() { printf("\033[2J\033[H"); }

void pause(char* str) {
  puts(*str ? str : "\nClique em qualquer tecla para continuar.");
  fflush(stdin);
  getchar();
  clear_console();
}

int show_options(const char** options, int options_count) {
  if (!options || !(*options) || options_count < 1) {
    puts("Nothing to show!");
    return -1;
  }

  for (int i = 0; i < options_count; ++i) printf("%2d - %s\n", i + 1, options[i]);

  return options_count;
}

void validate_option(int* option, int min, int max, char* error_message) {
  if (!error_message) error_message = "Invalid Option!";
  int valid = 1;
  do {
    if (!valid) puts(error_message);
    printf("[%d-%d]: ", min, max);

    fflush(stdin);
    valid = scanf("%d", option) && (*option >= min && *option <= max);
  } while (!valid);
}

int menu(int (*show_options)()) {
  clear_console();
  if (!show_options) {
    perror("Missing show options function!");
    exit(EXIT_FAILURE);
  }

  int options_count = show_options();
  // printf("%2d - Exit\n", ++options_count);
  int option;

  validate_option(&option, 1, options_count, NULL);

  fflush(stdin);
  return option;
}

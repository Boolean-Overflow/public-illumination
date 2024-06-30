#include "./utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearConsole() { printf("\033[2J\033[H"); }

void pause() {
  puts("\nClique na tecla <Enter> para continuar.");
  fflush(stdin);
  getchar();
  clearConsole();
}

int showOptions(const char** options, int options_count) {
  if (!options || !(*options) || options_count < 1) {
    puts("Nothing to show!");
    return -1;
  }

  for (int i = 0; i < options_count; ++i)
    printf("%2d - %s\n", i + 1, options[i]);

  return options_count;
}

void validateOption(int* option, int min, int max, char* error_message) {
  if (!error_message) error_message = "Opção inválida!";
  int valid = 1;
  do {
    if (!valid) puts(error_message);
    printf("[%d-%d]: ", min, max);

    fflush(stdin);
    valid = scanf("%d", option) && (*option >= min && *option <= max);
  } while (!valid);
}

int menu(int (*show_options)()) {
  clearConsole();
  if (!show_options) {
    perror("Missing show options function!");
    exit(EXIT_FAILURE);
  }

  int options_count = show_options();
  // printf("%2d - Exit\n", ++options_count);
  int option;

  validateOption(&option, 1, options_count, NULL);

  fflush(stdin);
  return option;
}

void readWord(char str[], size_t size) {
  fgets(str, size, stdin);
  str[strspn(str, "\n")] = '\0';
}

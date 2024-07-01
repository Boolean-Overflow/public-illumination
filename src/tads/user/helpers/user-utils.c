#include "user-utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../utils/utils.h"

/**
 * Validates if a username already exists in the AVL tree.
 * Returns true if the username does not exist, false otherwise.
 */
bool validateUsername(Avl* tree, char* username) {
  User* foundUser = findOne(tree, username);
  if (foundUser) {
    puts("Nome de utilizador já existente!");
    return false;
  }

  return true;
}

/**
 * Placeholder function for password validation.
 * Always returns true.
 */
bool isSpace(char* str) {
  while (*str != '\0') {
    if (*str == ' ') return true;
    str++;
  }
  return false;
}
bool validatePassword(char* password) {
  if (isSpace(password)) {
    printf("\n !! A password lida possui não deve possuir espaços !!\n");
    return false;
  }
  int len = 0;
  if (!inRange((len = strlen(password)), 4, 16)) {
    printf(
        "\n !! Password com tamanho \"%d\" inválido deve estar entre 4 e 16 "
        "!!\n",
        len);
    return false;
  }

  return true;
}

/**
 * Calculates a hash value based on the given user data (username and password).
 * Returns the hash value modulo 2024.
 */
hash_t hash(UserData user) {
  hash_t hash = 0;
  char* username = user.username;
  char* password = user.password;

  // Simple hash calculation using left shift and addition
  while (*username) hash = (hash << 5) + *username++;
  while (*password) hash = (hash << 5) + *password++;

  return hash % 2024;
}

/**
 * Creates a new User struct based on the provided user data and isAdmin flag.
 * Allocates memory for the user struct and sets username, hashed password,
 * and isAdmin status.
 * Returns a pointer to the newly created User struct.
 */
User* createUserUtil(UserData userData, bool isAdmin) {
  User* user = malloc(sizeof(User));
  if (!user) {
    perror("Erro de Alocação");  // Allocation error
    return NULL;
  }

  strcpy(user->username, userData.username);
  user->password = hash(userData);
  user->isAdmin = isAdmin;

  return user;
}

/**
 * Creates a new User struct based on the provided user data.
 * Calls createUserUtil with isAdmin set to false.
 * Returns a pointer to the newly created User struct.
 */
User* createUser(UserData userData) {
  // Call createUserUtil with isAdmin=false
  return createUserUtil(userData, false);
}

/**
 * Allocates memory for a User struct and copies data from the provided user
 * struct. Returns a pointer to the newly allocated User struct.
 */
User* allocateUser(User user) {
  User* allocatedUser = malloc(sizeof(User));
  if (!allocatedUser) {
    perror("Erro de Alocação");
    return NULL;
  }

  strcpy(allocatedUser->username, user.username);
  allocatedUser->password = user.password;
  allocatedUser->isAdmin = user.isAdmin;

  return allocatedUser;
}

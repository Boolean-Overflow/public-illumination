#include "user-utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool validateUsername(Avl* tree, char* username) {
  User* foundUser = findOne(tree, username);
  if (foundUser) {
    puts("Nome de utilizador já existente!");
    return false;
  }

  return true;
}

bool validatePassword(char* password) { return true; }

hash_t hash(UserData user) {
  hash_t hash = 0;
  char* username = user.username;
  char* password = user.password;

  while (*username) hash = (hash << 5) + *username++;
  while (*password) hash = (hash << 5) + *password++;

  return hash % 2024;
}

User* createUserUtil(UserData userData, bool isAdmin) {
  // Allocate memory for the user data
  User* user = malloc(sizeof(User));
  if (!user) {
    perror("Erro de Alocação");
    return NULL;
  }

  strcpy(user->username, userData.username);
  user->password = hash(userData);
  user->isAdmin = isAdmin;

  return user;
}

User* createUser(UserData userData) { return createUserUtil(userData, false); }

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
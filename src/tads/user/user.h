#ifndef __USER_H__
#define __USER_H__

#define MAX_USERNAME 20
#define MAX_PASSWORD 16
#define USERS_FILE "users.dat"
typedef unsigned long int hash_t;

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char username[MAX_USERNAME];
  char password[MAX_PASSWORD];
} UserData;

typedef struct {
  char username[MAX_USERNAME];
  hash_t password;
  bool isAdmin;
} User;

typedef struct Node {
  User* user;

  int height;
  struct Node *left, *right;
} Avl;

void insertUser(Avl** tree, UserData userData);

Avl* removeUser(Avl* tree, char* username);

User* findOne(Avl* tree, char* username);

void findAllUsers(Avl* tree, char* except);
void clearUsers(Avl** tree);

// User functions
Avl* loadUsers(long* count);
bool saveUser(User* user, FILE* stream);
void saveAllUsers(Avl* users);
void updateUser(Avl* users, UserData newUserData, char* oldPassword);

#endif
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

// Avl functions

/** Add a user to the AVL tree
 * @param users Pointer to the root pointer of the AVL tree
 * @param user user data to be added
 * @result Pointer to the root of the updated AVL tree
 */
void insertUser(Avl** tree, UserData userData);

/** Remove a user from the AVL tree
 * @param users Pointer to the root pointer of the AVL tree
 * @param username Key of the user to be removed
 * @result Pointer to the root of the updated AVL tree
 */
Avl* removeUser(Avl* users, char* username);

/** Find a user by their number (key)
 * @param users Pointer to the root pointer of the AVL tree
 * @param username Key of the user to be found
 * @return Pointer to the found user, or NULL if not found
 */
User* findOne(Avl* users, char* username);

/**
 * Print all users data
 * @param users: Pointer to the users root of the AVL tree
 */
void findAllUsers(Avl* users);
// bool clearUsers(Avl** users);

// User functions
Avl* loadUsers(long* count);
bool saveUser(User* user, FILE* stream);
void saveAllUsers(Avl* users);
void updateUser(Avl* users, UserData newUserData, char* oldPassword);

#endif
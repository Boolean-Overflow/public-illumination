#ifndef __USER_UTILS_H_
#define __USER_UTILS_H_

#include "../user.h"

bool validateUsername(Avl* tree, char* username);
bool validatePassword(char* password);

hash_t hash(UserData user);

User* createUser(UserData userData);
User* createUserUtil(UserData userData, bool isAdmin);
User* allocateUser(User user);

#endif
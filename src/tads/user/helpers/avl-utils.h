#ifndef __AVL_UTILS_H__
#define __AVL_UTILS_H__

#include "../user.h"

User* createUser(UserData userData);

Avl* createNode(User* user);

Avl* insertUserUtil(Avl* tree, User* user);

int getHeight(Avl* tree);

int updateHeight(Avl* node);

Avl* rotateLeft(Avl* x);

Avl* rotateRight(Avl* y);

Avl* minValueFrom(Avl* tree);

int getBalanceFactor(Avl* tree);

#endif
#ifndef __AVL_UTILS_H__
#define __AVL_UTILS_H__

#include "../user.h"

/**
 * Create a new user based on input data
 * @param user the data to be copied
 * @result Pointer to the newly created user node
 */
User* createUser(UserData userData);

Avl* createNode(User* user);

Avl* insertUserUtil(Avl* tree, User* user);

/**
 * Get height of a node in the AVL tree
 * @param tree pointer to the node whose height is to be obtained
 * @result Height of the node
 */
int getHeight(Avl* tree);

/**
 * Update height of a node
 * @param node Pointer to the node whose height is to be updated
 * @result Updated height of the node
 */
int updateHeight(Avl* node);

/**
 * Rotate subtree to the left
 * @param x: Pointer to the root of the subtree to be rotated
 * @returns Pointer to the new root of the rotated subtree
 */
Avl* rotateLeft(Avl* x);

/** Rotate subtree to the right
 * @param y Pointer to the root of the subtree to be rotated
 * @returns Pointer to the new root of the rotated subtree
 */
Avl* rotateRight(Avl* y);

// Find node with minimum value in a subtree
Avl* minValueFrom(Avl* tree);

// Get balance factor of a node
int getBalanceFactor(Avl* tree);

#endif
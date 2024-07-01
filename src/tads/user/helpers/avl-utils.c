#include "avl-utils.h"

#include "user-utils.h"

/**
 * Creates a new AVL node with the given user data.
 * Initializes left and right pointers to NULL,
 * assigns user data to the node, and sets initial height to 1.
 * Returns the created AVL node.
 */
Avl* createNode(User* user) {
  Avl* node = (Avl*)malloc(sizeof(Avl));
  if (!node) {
    perror("Erro de Alocação");
    return NULL;
  }

  node->left = node->right = NULL;
  if (!user) {
    free(node);
    return NULL;
  }
  node->user = user;  // Assign user data to the node

  node->height = 1;  // Set initial height to 1

  return node;
}

/**
 * Returns the height of the AVL tree rooted at 'tree'.
 * Returns 0 if 'tree' is NULL.
 */
int getHeight(Avl* tree) {
  if (!tree) return 0;
  return tree->height;
}

/**
 * Returns the maximum of two integers 'a' and 'b'.
 */
int max(int a, int b) { return a > b ? a : b; }

/**
 * Updates the height of the AVL node 'node'.
 * Recalculates the height based on its left and right subtrees.
 * Returns the updated height of 'node'.
 */
int updateHeight(Avl* node) {
  return max(getHeight(node->left), getHeight(node->right)) + 1;
}

/**
 * Performs a right rotation on AVL tree rooted at 'y'.
 * Returns the new root of the rotated subtree.
 */
Avl* rotateRight(Avl* y) {
  Avl* x = y->left;
  Avl* aux = x->right;

  // Perform rotation
  x->right = y;
  y->left = aux;

  // Update heights
  y->height = updateHeight(y);
  x->height = updateHeight(x);

  // Return new root
  return x;
}

/**
 * Performs a left rotation on AVL tree rooted at 'x'.
 * Returns the new root of the rotated subtree.
 */
Avl* rotateLeft(Avl* x) {
  Avl* y = x->right;
  Avl* aux = y->left;

  // Perform rotation
  y->left = x;
  x->right = aux;

  // Update heights
  x->height = updateHeight(x);
  y->height = updateHeight(y);

  // Return new root
  return y;
}

/**
 * Finds and returns the node with the minimum value in the AVL tree rooted at
 * 'root'. Returns NULL if 'root' is NULL.
 */
Avl* minValueFrom(Avl* root) {
  Avl* node = root;

  // Traverse leftmost nodes to find minimum value
  while (node->left) node = node->left;

  return node;
}

/**
 * Calculates and returns the balance factor of the AVL tree rooted at 'root'.
 * Balance factor is the difference between the heights of left and right
 * subtrees. Returns 0 if 'root' is NULL.
 */
int getBalanceFactor(Avl* root) {
  if (!root) return 0;

  return getHeight(root->left) - getHeight(root->right);
}

/**
 * Retrieves the key (username) associated with the AVL node 'node'.
 * Returns the username string.
 */
char* getKey(Avl* node) { return node->user->username; }

/**
 * Inserts a new user node into the AVL tree.
 * Utilizes recursive insertion based on username comparison.
 * Performs rotations and updates heights to maintain AVL balance.
 * Returns the updated AVL tree after insertion.
 */
Avl* insertUserUtil(Avl* tree, User* user) {
  if (!tree) return createNode(user);  // Create new node if tree is empty

  // Recursive insertion based on username comparison
  if (strcmp(user->username, getKey(tree)) > 0)
    tree->right = insertUserUtil(tree->right, user);
  else if (strcmp(user->username, getKey(tree)) < 0)
    tree->left = insertUserUtil(tree->left, user);
  else {
    puts("Utilizador já existente!");
    return tree;
  }

  tree->height = updateHeight(tree);

  int bf = getBalanceFactor(tree);

  // Perform rotations if tree becomes unbalanced
  if (bf > 1 && strcmp(user->username, getKey(tree->left)) < 0)
    return rotateRight(tree);

  if (bf < -1 && strcmp(user->username, getKey(tree->right)) > 0)
    return rotateLeft(tree);

  if (bf > 1 && strcmp(user->username, getKey(tree->left)) > 0) {
    tree->left = rotateLeft(tree->left);
    return rotateRight(tree);
  }

  if (bf < -1 && strcmp(user->username, getKey(tree->right)) < 0) {
    tree->right = rotateRight(tree->right);
    return rotateLeft(tree);
  }

  return tree;
}

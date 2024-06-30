#include "avl-utils.h"

#include "user-utils.h"

Avl* createNode(User* user) {
  Avl* node = (Avl*)malloc(sizeof(Avl));
  if (!node) {
    perror("Erro de Alocação");
    return NULL;
  }
  // Initialize left and right pointers to NULL
  node->left = node->right = NULL;

  // Assign student data to the node
  if (!user) {
    free(node);
    return NULL;
  }
  node->user = user;

  // Set initial height to 1
  node->height = 1;

  return node;
}

int getHeight(Avl* tree) {
  if (!tree) return 0;
  return tree->height;
}

int max(int a, int b) { return a > b ? a : b; }
int updateHeight(Avl* node) {
  return max(getHeight(node->left), getHeight(node->right)) + 1;
}

Avl* rotateRight(Avl* y) {
  Avl* x = y->left;
  Avl* aux = x->right;

  // Rotate
  x->right = y;
  y->left = aux;

  // Update height
  y->height = updateHeight(y);
  x->height = updateHeight(x);

  // New root
  return x;
}

Avl* rotateLeft(Avl* x) {
  Avl* y = x->right;
  Avl* aux = y->left;

  // Rotate
  y->left = x;
  x->right = aux;

  // Update height
  x->height = updateHeight(x);
  y->height = updateHeight(y);

  // New root
  return y;
}

Avl* minValueFrom(Avl* root) {
  Avl* node = root;

  while (node->left) node = node->left;

  return node;
}

int getBalanceFactor(Avl* root) {
  if (!root) return 0;

  return getHeight(root->left) - getHeight(root->right);
}

char* getKey(Avl* node) { return node->user->username; }

// AVL methods

Avl* insertUserUtil(Avl* tree, User* user) {
  if (!tree) return createNode(user);

  // Recursive insertion
  if (strcmp(user->username, getKey(tree)) > 0)
    tree->right = insertUserUtil(tree->right, user);
  else if (strcmp(user->username, getKey(tree)) < 0)
    tree->left = insertUserUtil(tree->left, user);
  else {
    puts("Utilizador já existente!");
    return tree;
  }

  // Update height
  tree->height = updateHeight(tree);

  // Check balance
  int bf = getBalanceFactor(tree);

  // Perform rotations if needed
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

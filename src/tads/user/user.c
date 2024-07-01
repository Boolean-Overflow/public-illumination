#include "user.h"

#include "helpers/avl-utils.h"
#include "helpers/user-utils.h"

/**
 * Inserts a new user into the AVL tree.
 * Creates a user node, inserts it into the tree,
 * and saves the user data to file.
 */
void insertUser(Avl** tree, UserData userData) {
  User* user = createUser(userData);
  *tree = insertUserUtil(*tree, user);
  saveUser(user, NULL);
}

/**
 * Removes a user from the AVL tree by username.
 * Returns the updated AVL tree after removal.
 */
Avl* removeUser(Avl* tree, char* username) {
  if (!tree) {  // Return NULL if tree does not exist
    puts("Utilizador não encontrado!");
    return tree;
  }

  // Recursive removal based on username comparison
  if (strcmp(username, tree->user->username) < 0)
    tree->left = removeUser(tree->left, username);
  else if (strcmp(username, tree->user->username) > 0)
    tree->right = removeUser(tree->right, username);
  else {
    if (tree->left && tree->right) {  // If both children exist
      Avl* tmp = minValueFrom(tree->right);
      tree->user = tmp->user;  // Copy data from successor node
      tree->right = removeUser(tree->right, tmp->user->username);
    } else {
      Avl* child = tree->left ? tree->left : tree->right;
      if (!child) {
        child = tree;
        tree = NULL;
      } else
        *tree = *child;

      free(child);
      printf("Utilizador %s removido com sucesso\n", username);
    }
  }

  if (!tree) return tree;  // Return if tree is empty

  tree->height = updateHeight(tree);  // Update tree height
  int bf = getBalanceFactor(tree);    // Get balance factor

  // Perform rotations based on balance factor
  if (bf > 1 && getBalanceFactor(tree->left) >= 0) return rotateRight(tree);
  if (bf > 1 && getBalanceFactor(tree->left) < 0) {
    tree->left = rotateLeft(tree->left);
    return rotateRight(tree);
  }
  if (bf < -1 && getBalanceFactor(tree->right) <= 0) return rotateLeft(tree);
  if (bf < -1 && getBalanceFactor(tree->right) > 0) {
    tree->right = rotateRight(tree->right);
    return rotateLeft(tree);
  }

  return tree;
}

/**
 * Finds and returns a user from the AVL tree by username.
 * Returns NULL if user is not found.
 */
User* findOne(Avl* tree, char* username) {
  if (!tree) return NULL;

  // Recursive search based on username comparison
  if (strcmp(username, tree->user->username))
    return findOne(tree->right, username);

  if (strcmp(username, tree->user->username))
    return findOne(tree->left, username);

  return tree->user;
}

/**
 * Prints all usernames in the AVL tree in order,
 * excluding a specified username (except).
 */
void findAllUsers(Avl* tree, char* except) {
  if (!tree) return;

  // In-order traversal and print, excluding 'except'
  findAllUsers(tree->left, except);
  if (strcmp(except, tree->user->username) != 0) puts(tree->user->username);
  findAllUsers(tree->right, except);
}

/**
 * Clears all users from the AVL tree and frees memory.
 * Sets tree to NULL after clearing.
 */
void clearUsers(Avl** tree) {
  if (!tree || !(*tree)) return;

  // Post-order traversal to free nodes and user data
  clearUsers(&(*tree)->left);
  clearUsers(&(*tree)->right);

  free((*tree)->user);
  free(*tree);

  *tree = NULL;
}

/**
 * Loads users from file into an AVL tree.
 * Returns the AVL tree with loaded users.
 */
Avl* loadUsers(long* count) {
  FILE* file = fopen(USERS_FILE, "a+");  // Open file for reading and appending
  if (!file) {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", USERS_FILE);
    return NULL;
  }

  // Append admin user if file is empty
  long size = ftell(file);
  if (size == 0L) {
    UserData admin = {"admin", "admin"};
    saveUser(createUserUtil(admin, true), file);
  }

  rewind(file);  // Rewind file pointer to start
  // Load users from file into AVL tree
  int res;
  User user;
  Avl* tree = NULL;
  while ((res = fscanf(file, "%s %lu %hu", user.username, &user.password,
                       (short*)(&user.isAdmin))) &&
         (res == 3 && res != EOF)) {
    tree = insertUserUtil(tree, allocateUser(user));
    if (count) *count += 1;
  }

  fclose(file);  // Close file after reading

  return tree;
}

/**
 * Saves a user's data to file or stream.
 * Returns true if save operation is successful, false otherwise.
 */
bool saveUser(User* user, FILE* stream) {
  if (!user) {
    perror("Sem dados para salvar!");
    return false;
  }
  // Open file or use provided stream
  FILE* file = stream ? stream : fopen(USERS_FILE, "a+");
  if (!file) {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", USERS_FILE);
    return false;
  }

  // Write user data to file
  fprintf(file, "%s %lu %hu\n", user->username, user->password, user->isAdmin);

  if (!stream) fclose(file);  // Close file if not using stream

  return true;
}

/**
 * Saves all users from AVL tree to file.
 * Writes all user data in order to the specified stream.
 */
void saveAllUsersUtil(Avl* tree, FILE* stream) {
  if (!tree) return;

  // In-order traversal to save all users
  saveAllUsersUtil(tree->left, stream);
  saveUser(tree->user, stream);
  saveAllUsersUtil(tree->right, stream);
}

/**
 * Saves all users from AVL tree to file.
 * Opens a new file for writing and saves all users.
 */
void saveAllUsers(Avl* users) {
  if (!users) return;
  FILE* file = fopen(USERS_FILE, "w");  // Open file for writing

  saveAllUsersUtil(users, file);  // Save all users to file

  fclose(file);  // Close file after saving
}

/**
 * Updates a user's data in the AVL tree.
 * Updates user's password if oldPassword matches.
 */
void updateUser(Avl* tree, UserData newUserData, char* oldPassword) {
  if (!tree) return;

  // Find user in AVL tree
  User* foundUser = findOne(tree, newUserData.username);
  if (!foundUser) {
    fprintf(stderr, "Utilizador não encontrado!");
    return;
  }
  UserData user = newUserData;
  strcpy(user.password, oldPassword);

  // Check if old password matches
  if (hash(user) != foundUser->password) {
    fprintf(stderr, "Utilizador não encontrado!");
    return;
  }

  foundUser->password = hash(newUserData);  // Update user password

  saveAllUsers(tree);  // Save all users after update
}

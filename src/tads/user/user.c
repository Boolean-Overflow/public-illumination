#include "user.h"

#include "helpers/avl-utils.h"
#include "helpers/user-utils.h"

void insertUser(Avl** tree, UserData userData) {
  User* user = createUser(userData);
  *tree = insertUserUtil(*tree, user);
  saveUser(user, NULL);
}

Avl* removeUser(Avl* tree, char* username) {
  // Return NULL if tree does not exist
  if (!tree) {
    puts("Utilizador não encontrado!");
    return tree;
  }

  // Recursive removal
  if (strcmp(username, tree->user->username) < 0)
    tree->left = removeUser(tree->left, username);
  else if (strcmp(username, tree->user->username) > 0)
    tree->right = removeUser(tree->right, username);
  else {
    // Checking if both children exist
    if (tree->left && tree->right) {
      Avl* tmp = minValueFrom(tree->right);

      // copy data
      tree->user = tmp->user;

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

  // If the tree became empty
  if (!tree) return tree;

  // Update height
  tree->height = updateHeight(tree);

  // Check balance
  int bf = getBalanceFactor(tree);

  // Perform rotations if needed
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

User* findOne(Avl* tree, char* username) {
  if (!tree) return NULL;

  // Recursive search
  if (strcmp(username, tree->user->username))
    return findOne(tree->right, username);

  if (strcmp(username, tree->user->username))
    return findOne(tree->left, username);

  return tree->user;
}

void findAllUsers(Avl* users) {
  if (!users) return;

  // Print in order
  findAllUsers(users->left);
  puts(users->user->username);
  findAllUsers(users->right);
}

// bool clearUsers(Avl** users) {}

// User functions
Avl* loadUsers(long* count) {
  FILE* file = fopen(USERS_FILE, "a+");
  if (!file) {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", USERS_FILE);
    return NULL;
  }

  // if empty append the admin
  long size = ftell(file);

  if (size == 0L) {
    UserData admin = {"admin", "admin"};
    saveUser(createUserUtil(admin, true), file);
  }

  rewind(file);
  // loading users from file
  int res;
  User user;
  Avl* tree = NULL;
  while ((res = fscanf(file, "%s %lu %hu", user.username, &user.password,
                       (short*)(&user.isAdmin))) &&
         (res == 3 && res != EOF)) {
    tree = insertUserUtil(tree, allocateUser(user));
    if (count) *count += 1;
  }

  fclose(file);

  return tree;
}

bool saveUser(User* user, FILE* stream) {
  if (!user) {
    perror("Sem dados para salvar!");
    return false;
  }

  FILE* file = stream ? stream : fopen(USERS_FILE, "a+");
  if (!file) {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", USERS_FILE);
    return false;
  }

  fprintf(file, "%s %lu %hu\n", user->username, user->password, user->isAdmin);

  if (!stream) fclose(file);

  return true;
}

void saveAllUsersUtil(Avl* tree, FILE* stream) {
  if (!tree) return;

  saveAllUsersUtil(tree->left, stream);
  saveUser(tree->user, stream);
  saveAllUsersUtil(tree->right, stream);
}

void saveAllUsers(Avl* users) {
  if (!users) return;
  FILE* file = fopen(USERS_FILE, "w");

  saveAllUsersUtil(users, file);

  fclose(file);
}

void updateUser(Avl* tree, UserData newUserData, char* oldPassword) {
  if (!tree) return;

  User* foundUser = findOne(tree, newUserData.username);
  if (!foundUser) {
    fprintf(stderr, "Utilizador não encontrado!");
    return;
  }
  UserData user = newUserData;
  strcpy(user.password, oldPassword);

  if (hash(user) != foundUser->password) {
    fprintf(stderr, "Utilizador não encontrado!");
    return;
  }

  foundUser->password = hash(newUserData);

  saveAllUsers(tree);
}
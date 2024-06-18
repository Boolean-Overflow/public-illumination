#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int userExists(User users[], int count, char* username) {
  for (int i = 0; i < count; ++i) {
    if (strcmp(users[i].username, username) == 0) {
      return i;
    }
  }
  return -1;
}

void addUser(User users[], int* count, char* username, char* password, int hashValue, int isAdm) {
  strcpy(users[*count].username, username);
  strcpy(users[*count].password, password);
  users[*count].hashValue = hashValue;
  users[*count].isAdm = isAdm;
  (*count)++;
  printf("Usuário criado com sucesso.\n");
}

void saveAllUsers(User users[], int count) {
  FILE* fp = fopen(USER_FILE, "w");
  if (fp == NULL) {
    perror("Erro ao abrir arquivo.");
    return;
  }

  for (int i = 0; i < count; ++i) {
    fprintf(fp, "%s %u %d\n", users[i].username, users[i].hashValue, users[i].isAdm);
  }

  fclose(fp);
}

void saveUser(char* username, unsigned int hashValue, int isAdm) {
  FILE* fp = fopen(USER_FILE, "a+");
  if (fp == NULL) {
    perror("Erro ao abrir arquivo.");
    return;
  }
  fprintf(fp, "%s %u %d\n", username, hashValue, isAdm);
  fclose(fp);
}

int loadUsers(User users[]) {
  FILE* fp = fopen(USER_FILE, "r");
  if (fp == NULL) {
    perror("Erro ao abrir arquivo.");
    return 0;
  }

  int count = 0;
  while (fscanf(fp, "%s %u %d\n", users[count].username, &users[count].hashValue, &users[count].isAdm) != EOF) {
    count++;
  }

  fclose(fp);
  return count;
}
int isSpace(char* str) {
  while (*str != '\0') {
    if (*str == ' ') return 1;
    str++;
  }
  return 0;
}

void getUserName(User users[], char* username, int count) {
  printf("Digite o nome de usuario: ");
  scanf("%s", username);

  while (userExists(users, count, username) != -1 || isSpace(username)) {
    printf("Nome de usuario invalido.\nDigite outro nome de usuario: ");
    scanf("%s", username);
  }
}

void getPassWord(char* password) {
  printf("Digite a Palavra-Passe do usuario: ");
  scanf("%s", password);

  while (isSpace(password)) {
    printf("Palavra-Passe Inválida.\nDigite novamente a Palavra-Passe: ");
    scanf("%s", password);
  }
}

unsigned int hashF(char* username, char* password) {
  unsigned int hash = 0;
  int c;
  while ((c = *username++)) {
    hash = hash * 31 + c;
  }
  while ((c = *password++)) {
    hash = hash * 31 + c;
  }
  return hash % MAX_USER;
}

void createUser(User users[], int* userCount) {
  char username[MAX_USERNAME];
  char password[MAX_PASSWORD];
  getUserName(users, username, *userCount);
  getPassWord(password);
  int hashValue = hashF(username, password);
  int isAdm = 0;
  addUser(users, userCount, username, password, hashValue, isAdm);
  saveUser(username, hashValue, isAdm);
}

User* login(User users[], int userCount) {
  User* user = malloc(sizeof(User));
  char username[MAX_USERNAME];
  printf("Digite o nome de usuario: ");
  scanf("%s", username);
  int i = userExists(users, userCount, username);
  if (i == -1) {
    printf("Nome de usuario Invalido!");
    return NULL;
  }
  char password[MAX_PASSWORD];
  getPassWord(password);
  unsigned int hashValue = hashF(username, password);
  if (users[i].hashValue == hashValue) {
    printf("Acesso a conta!");
    user = &(users[i]);
    return user;
  } else {
    printf("Password Invalida!");
    return NULL;
  }
}

void editUsername(User users[], int Usercount, int pos) {
  if (pos == -1) {
    printf("Usuario Inexistente!");
    return;
  }
  char newName[MAX_USERNAME];
  getUserName(users, newName, Usercount);
  unsigned int newHash = hashF(newName, users[pos].password);
  strcpy(users[pos].username, newName);
  users[pos].hashValue = newHash;
  saveAllUsers(users, Usercount);
}

void editUserpassword(User users[], int Usercount, int pos) {
  if (pos == -1) {
    printf("Usuario Inexistente!");
    return;
  }
  char newPassword[MAX_PASSWORD];
  getPassWord(newPassword);
  unsigned int newHash = hashF(users[pos].username, newPassword);
  strcpy(users[pos].password, newPassword);
  users[pos].hashValue = newHash;
  saveAllUsers(users, Usercount);
}

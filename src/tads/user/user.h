#ifndef __USER__
#define __USER__

#define MAX_USERNAME 50
#define MAX_PASSWORD 16
#define MAX_USER 100
#define ATTEMPTS 3
#define USER_FILE "user.txt"

typedef struct {
  char username[MAX_USERNAME];
  char password[MAX_PASSWORD];
  unsigned int hashValue;
  int isAdm;
} User;

int userExists(User users[], int count, char* username);
void addUser(User users[], int* count, char* username, char* password, int hashValue, int isAdm);
void saveAllUsers(User users[], int count);
void saveUser(char* username, unsigned int hashValue, int isAdm);
int loadUsers(User users[]);
int isSpace(char* str);
void getUserName(User users[], char* username, int count);
void getPassWord(char* password);
void createUser(User users[], int* userCount);
User* login(User users[], int userCount);
void editUsername(User users[], int Usercount, int pos);
void editUserpassword(User users[], int Usercount, int pos);
#endif
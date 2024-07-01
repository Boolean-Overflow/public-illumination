#include "auth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../tads/user/helpers/user-utils.h"
#include "../../utils/utils.h"

User* login(Avl* tree) {
  UserData user;
  puts("======== Login ========\n");
  int attempts = 3;

  while (attempts--) {
    if ((attempts + 1) < 3) printf("Restam-lhe %d tentativas!\n", attempts + 1);

    puts("Informe com precisão os dados abaixo.");

    printf("Nome de utilizador: ");
    fflush(stdin);
    scanf("%19[^ \n]", user.username);

    printf("Senha: ");
    fflush(stdin);
    scanf("%15[^ \n]", user.password);

    User* foundUser = findOne(tree, user.username);
    if (!foundUser || hash(user) != foundUser->password) {
      puts("Credenciais erradas!");
      continue;
    }

    return foundUser;
  }

  puts("Excedeu o limite de tentativas!");
  return NULL;
}

void signup(Avl** tree) {
  UserData user;

  do {
    printf("Informe o nome de utilizador\n[4-20 caracteres]: ");
    scanf("%19[^ \n]", user.username);
    fflush(stdin);
  } while (!validateUsername(*tree, user.username));

  do {
    printf("Informe a senha de utilizador\n[8-16 caracteres]: ");
    scanf("%15[^ \n]", user.password);
    fflush(stdin);
  } while (!validatePassword(user.password));

  insertUser(tree, user);

  puts("Utilizador criado com sucesso!\n Já pode efectuar o seu login");
}

int auth_menu() {
  puts("======= AUTENTICAÇÃO ========\n");

  const char* options[] = {"Entrar", "Registar-se", "Sair"};
  return showOptions(options, sizeof(options) / sizeof(options[0]));
}
User* authUseCase(Avl** tree) {
  while (true) {
    clearConsole();
    User* user = NULL;
    int option = menu(auth_menu);
    clearConsole();

    switch (option) {
      case 1:
        user = login(*tree);
        if (user) return user;
        break;
      case 2:
        puts("======== SignUp ========");
        signup(tree);
        pause();
        break;
      case 3:
        return NULL;
    }
  }
}

int users_menu() {
  puts("========== GERIR UTILIZADORES ==========\n");

  const char* options[] = {"Listar utilizadores", "Criar utilizador",
                           "Eliminar utilizador", "Voltar"};

  return showOptions(options, sizeof(options) / sizeof(options[0]));
}

void handleUsers(Avl** tree) {
  while (true) {
    int option = menu(users_menu);

    switch (option) {
      case 1: {
        puts("========== UTILIZADORES CADASTRADOS ==========\n");
        findAllUsers(*tree, "admin");
      } break;
      case 2: {
        puts("========== CRIAR UTILIZADOR ==========\n");
        signup(tree);
      } break;
      case 3: {
        puts("========== ELIMINAR UTILIZADOR ==========\n");
        char username[MAX_USERNAME];
        do {
          if (strcmp(username, "admin") == 0)
            puts(
                "!! Se deseja eliminar a conta acesse o seu perfil! "
                "!!\n== Informe um novo ou digite qualquer character para"
                "terminar == \n");

          printf("Nome de utilizador: ");
          scanf("%19[^ \n]", username);
          fflush(stdin);
        } while (strcmp(username, "admin") == 0);

        *tree = removeUser(*tree, username);
        saveAllUsers(*tree);

      } break;
      case 4:
        return;
    }
    pause();
  }
}

int profile_menu() {
  const char* options[] = {"Modificar Nome", " Modificar Senha ",
                           " Eliminar Conta ", " Voltar "};
  return showOptions(options, sizeof(options) / sizeof(options[0]));
}

bool confirmPassword(User* user) {
  UserData userdata;
  int attempts = 3;
  strcpy(userdata.username, user->username);
  hash_t hash_value = 0;
  do {
    printf("Senha: ");
    scanf("%15[^ \n]", userdata.password);
    fflush(stdin);
    hash_value = hash(userdata);
    if (hash_value != user->password) {
      printf("Senha de utilizador invalida! Restam-lhe %d tentativas!\n",
             attempts--);
    }
  } while (attempts != 0 && hash_value != user->password);

  return (hash_value == user->password);
}

void getNewPassword(Avl** tree, User* user) {
  if (confirmPassword(user)) {
    do {
      printf("Informe a nova senha de utilizador\n[8-16 caracteres]: ");
      scanf("%15[^ \n]", user->password);
      fflush(stdin);
    } while (!validatePassword(user->password));
    saveAllUsers(*tree);
  }
}

void getNewUserName(Avl** tree, User* user) {
  if (confirmPassword(user)) {
    do {
      printf("Informe o nome de utilizador\n[4-20 caracteres]: ");
      scanf("%19[^ \n]", user->username);
      fflush(stdin);
    } while (!validateUsername(*tree, user->username));
    saveAllUsers(*tree);
  }
}

void eliminateUser(Avl** tree, User* user) {
  if (confirmPassword(user)) {
    removeUser(*tree, user->username);
    saveAllUsers(*tree);
  }
}
int handleProfile(Avl** tree, User* user) {
  puts("Perfil");
  int option = menu(profile_menu);
  switch (option) {
    case 1:
      getNewUserName(tree, user);
      break;
    case 2:
      getNewPassword(tree, user);
      break;
    case 3:
      eliminateUser(tree, user);
      break;
    case 4:
      return 0;
      break;
  }
  pause();
  return 1;
}

int usersUseCase(Avl** tree, User* user) {
  while (true) {
    clearConsole();
    int i = 1, option;
    printf("\nBem vindo(a) Sr(a). %s\n\n", user->username);
    if (user->isAdmin) printf("%d - Gerir Utilizadores\n", i++);
    printf("%d - Perfil\n", i++);
    printf("%d - Ir para a Simulação\n", i++);
    printf("%d - Encerrar sessão\n", i);

    validateOption(&option, 1, i, NULL);

    if (option == 1 && user->isAdmin) {
      handleUsers(tree);
    } else if (option == 1 || (option == 2 && user->isAdmin)) {
      if (handleProfile(tree, user) == 0) return 0;
    } else if (option == 2 || (option == 3 && user->isAdmin)) {
      return 1;
    } else {
      return 0;
    }
  }
}
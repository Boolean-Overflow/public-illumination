#include "auth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../tads/user/helpers/user-utils.h"
#include "../../utils/utils.h"

User* login(Avl* tree) {
  UserData user;
  clearConsole();
  puts("======== Login ========");
  int attempts = 3;

  while (attempts--) {
    if ((attempts + 1) < 3) printf("Restam-lhe %d tentativas!\n", attempts + 1);

    puts("Informe com precisão os dados abaixo.");

    printf("Nome de utilizador:");
    scanf("%19[^ \n]", user.username);
    fflush(stdin);

    printf("Senha: ");
    scanf("%15[^ \n]", user.password);
    fflush(stdin);

    User* foundUser = findOne(tree, user.username);
    if (!foundUser || hash(user) != foundUser->password) {
      puts("Credenciais erradas!");
      continue;
    }

    printf("Bem vindo(a) Sr(a). %s\n", foundUser->username);
    pause();
    return foundUser;
  }

  puts("Excedeu o limite de tentativas!");
  return NULL;
}

void signup(Avl** tree) {
  UserData user;
  clearConsole();
  puts("======== SignUp ========");

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
  pause();
}

int auth_menu() {
  puts("======= AUTENTICAÇÃO ========");

  const char* options[] = {"Entrar", "Registar-se", "Sair"};
  return showOptions(options, sizeof(options) / sizeof(options[0]));
}
User* authUseCase(Avl** tree) {
  while (true) {
    clearConsole();
    User* user = NULL;
    int option = menu(auth_menu);

    switch (option) {
      case 1:
        user = login(*tree);
        if (user) return user;
        break;
      case 2:
        signup(tree);
        break;
      case 3:
        return NULL;
    }
  }
}

void handleUsers(Avl** tree) {
  puts("Gerir Utilizadores");
  pause();
}

int handleProfile(Avl** tree, User* user) {
  puts("Perfil");
  pause();
  return 1;
}

int usersUseCase(Avl** tree, User* user) {
  while (true) {
    clearConsole();
    int i = 1, option;
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
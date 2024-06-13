#include <paths.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/utils.h"

int auth_menu() {
  puts("======= AUTENTICAÇÃO ========");
  const char* options[] = {"Entrar", "Registar-se", "Sair"};

  return show_options(options, sizeof(options) / sizeof(options[0]));
}

int app_menu() {
  puts("======= ILUMINAÇÃO PÚBLICA ========");
  const char* options[] = {"Criar Localidade",   "Adicionar poste", "Remover Poste",
                           "Adicionar nova rua", "Eliminar Rua",    "Actualizar distância entre postes",
                           "Iluminação Total",   "Melhor solução",  "Iluminar ruas adjacentes",
                           "Destruir simulação", "Encerrar sessão"};

  return show_options(options, sizeof(options) / sizeof(options[0]));
}

int main() {
auth: {
  int option = menu(auth_menu);

  switch (option) {
    case 1: {
      puts("Sign In");
      pause("");
      goto app;
    } break;
    case 2: {
      puts("Sign Up");
    } break;
    default:
      goto end;
  }
  goto auth;
}
app: {
  int option = menu(app_menu);
  switch (option) {
    case 1: {
      char filename[255];
      printf("Informe o nome do arquivo de entrada: ");
      scanf("%254[^ \n]", filename);
      fflush(stdin);
      printf("Ficheiro \"./%s\" correcto?", filename);
      pause("");

      FILE* file = fopen(filename, "r");
      if (!file) {
        perror("FILE_ERROR");
        exit(EXIT_FAILURE);
      }

      int v, a;
      fscanf(file, "%d %d", &v, &a);
      // inicializar graph
      int start, end;
      float distance;
      while (fscanf(file, "%d %d %f", &start, &end, &distance) != EOF) {
        if (start == 0 && end == 0) break;
        // Adicionar rua
        printf(" [%d] <%.2f> [%d]\n", start, distance, end);
      }

      fclose(file);
    } break;
    case 11:
      goto auth;
      break;
    default:
      printf("Opção %d\n", option);
      goto app;
  }
}
end:
  puts("Obrigado ;)\nBy: Grupo 2");
  puts("Matateu André - 20212549🤓");
  puts("Lukeny Silva - 20220718😎");
  puts("Kélsio Mateus -  20221473🧑‍💻");

  return 0;
}
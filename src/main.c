#include <paths.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tads/graph/graph.h"
#include "tads/user/user.h"
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
  Graph* graph = NULL;

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

      int vertices, edges;
      fscanf(file, "%d %d", &vertices, &edges);
      graph = createGraph(vertices);

      int srcPost, destinationPost;
      float distance;
      while (fscanf(file, "%d %d %f", &srcPost, &destinationPost, &distance) != EOF) {
        if (srcPost == 0 && destinationPost == 0) break;

        graph = addStreet(graph, srcPost, destinationPost, distance);
      }

      fclose(file);

      printf("Ficheiro \"%s\" carregado com sucesso!", filename);
      illuminateStreets(graph);
    } break;
    case 3: {
      puts("===============REMOVER POSTE=================");
      int post;
      printf("Informe o valor do poste: ");
      validate_option(&post, 1, graph->totalPosts, "Valor de poste inválido!");
      graph = removePost(graph, post);

      printf("Poste %d removido!", post);
    } break;
    case 7: {
      puts("=========LOCALIDADE=========");
      illuminateStreets(graph);
    } break;
    case 10: {
      puts("=============DESTRUIR SIMULAÇÃO============");
      graph = destroyGraph(graph);
      if (graph) perror("Ocorreu um erro ao destruir a simulação!"), exit(EXIT_FAILURE);

      puts("Simulação destruida com sucesso!");
    } break;
    case 11:
      goto auth;
      break;
  }
  pause("");
  goto app;
}
end:
  puts("Obrigado ;)\nBy: Grupo 2");
  puts("Matateu André - 20212549🤓");
  puts("Lukeny Silva - 20220718😎");
  puts("Kélsio Mateus -  20221473🧑‍💻");

  return 0;
}
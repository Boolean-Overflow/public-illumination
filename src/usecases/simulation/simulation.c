#include "simulation.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"

Graph* loadPlace() {
  char filename[255];
  do {
    fflush(stdin);
    printf("Informe o nome do arquivo de entrada: ");
    scanf("%254[^ \n]", filename);
    fflush(stdin);
    printf("Ficheiro \"./%s\" correcto?[S/N]: ", filename);
  } while (toupper(getchar()) == 'N');

  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Erro ao abrir o ficheiro \"%s\"\n", filename);
    exit(EXIT_FAILURE);
  }

  int vertices, edges;
  fscanf(file, "%d %d", &vertices, &edges);
  Graph* graph = createGraph(vertices);

  int srcPost, destinationPost;
  float distance;
  while (fscanf(file, "%d %d %f", &srcPost, &destinationPost, &distance) !=
         EOF) {
    if (srcPost == 0 && destinationPost == 0) break;

    graph = addStreet(graph, srcPost, destinationPost, distance);
  }

  fclose(file);

  printf("Ficheiro \"%s\" carregado !", filename);

  return graph;
}

void hadleAddPost(Graph** graph) { *graph = addPost(*graph); }

void handleRemovePost(Graph** graph) {
  int srcPost = 0;
  printf("Informe o valor do poste: ");
  validateOption(&srcPost, 1, (*graph)->totalPosts, NULL);

  *graph = removePost(*graph, srcPost);

  printf("Post %d removido com sucesso!\n", srcPost);
}

void handleAddStreet(Graph** graph) {
  int src, dest, valid = 1;
  float distance;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, (*graph)->totalPosts, NULL);
  printf("Informe o poste de destino: ");
  validateOption(&dest, 1, (*graph)->totalPosts, NULL);
  do {
    if (!valid) puts("Valor inválido!");

    printf("Informe a distancia entre os postes: ");

  } while ((valid = !scanf("%f", &distance)));

  *graph = addStreet(*graph, src, dest, distance);

  printf("Rua de %d à %d distando %.2fm adicionada !", src, dest, distance);
}
void handleRemoveStreet(Graph** graph) {
  int src, dest;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, (*graph)->totalPosts, NULL);

  printf("Informe o poste de destino: ");
  validateOption(&dest, 1, (*graph)->totalPosts, NULL);

  *graph = removeStreet(*graph, src, dest);

  printf("Rua de %d à %d removida !", src, dest);
}

void handleUpdateDistance(Graph** graph) {
  int src, dest, valid = 1;
  float newDistance;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, (*graph)->totalPosts, NULL);
  printf("Informe o poste de destino: ");
  validateOption(&dest, 1, (*graph)->totalPosts, NULL);

  do {
    if (!valid) puts("Valor inválido!");

    printf("Informe a distancia entre os postes: ");

  } while ((valid = !scanf("%f", &newDistance)));

  *graph = updateDistance(*graph, src, dest, newDistance);
  printf("\n");
}
void handleTotalIlumination(Graph* graph) {
  puts("Total Illumination");
  float total = illuminateStreets(graph);
  printf("Custo total: %.2fKzs", total * COST_PER_METER);
}
void handleBestSolution(Graph* graph) {
  // MST
  float totalCost = illuminateStreets(graph) * COST_PER_METER;
  printf("\n\n");
  float bestCost = primMST(graph) * COST_PER_METER;
  float f = totalCost - bestCost;
  printf(
      "\n\nCusto total: %.2fKzs\nCusto eficiente: "
      "%.2fKzs\nValor poupado: %.2fKzs[%.1f%%]\n",
      totalCost, bestCost, f, (f / totalCost) * 100);
}
void handleIlluminateAdjacentStreet(Graph* graph) {
  // DIJKSTRA
  puts("Illuminate Adjacent Streets");
}
void handleDestroySimulation(Graph** graph) {
  puts("=============DESTRUIR SIMULAÇÃO============");
  *graph = destroyGraph(*graph);

  if (*graph)
    perror("Ocorreu um erro ao destruir a simulação!"), exit(EXIT_FAILURE);

  puts("Simulação destruida com sucesso!");
}

int simulation_menu() {
  const char* options[] = {
      "Carregar localidade", "Adicionar poste",
      "Remover poste",       "Adicionar rua",
      "Remover rua",         "Actualizar distância entre postes",
      "Destruir simulação",  "Voltar"};
  return showOptions(options, sizeof(options) / sizeof(options[0]));
}
void handleSimulation(Graph** graph) {
  bool out = false;
  while (!out) {
    int option = menu(simulation_menu);
    switch (option) {
      case 1:
        *graph = loadPlace();
        break;
      case 2:
        hadleAddPost(graph);
        break;
      case 3:
        handleRemovePost(graph);
        break;
      case 4:
        handleAddStreet(graph);
        break;
      case 5:
        handleRemoveStreet(graph);
        break;
      case 6:
        handleUpdateDistance(graph);
        break;
      case 7:
        handleDestroySimulation(graph);
        break;
      default:
        out = true;
        break;
    }
    if (!out) pause();
  }
}

bool simulationUseCase(Graph** mainGraph, User* user) {
  Graph* graph = *mainGraph;
  while (true) {
    clearConsole();
    int i = 1, option;

    if (!graph) puts("Grafo Inexistente");

    if (user->isAdmin) printf("%d - Gerir Simulação\n", i++);
    if (graph) {
      printf("%d - Iluminação da localidade\n", i++);
      printf("%d - Melhor solução de iluminação da localidade\n", i++);
      printf("%d - Iluminação de ruas adjacentes\n", i++);
    }

    printf("%d - Encerrar sessão\n", i);

    validateOption(&option, 1, i, NULL);

    if (option == 1 && user->isAdmin) {
      handleSimulation(&graph);
    } else if ((option == 1 || (option == 2 && user->isAdmin)) && graph) {
      handleTotalIlumination(graph);
    } else if ((option == 2 || (option == 3 && user->isAdmin)) && graph) {
      handleBestSolution(graph);
    } else if ((option == 3 || (option == 4 && user->isAdmin)) && graph) {
      handleIlluminateAdjacentStreet(graph);
    } else {
      break;
    }

    pause();
  }

  *mainGraph = graph;
  return false;
}
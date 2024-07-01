#include "simulation.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"

/**
 * Loads a graph representing a place from a file.
 * Prompts the user for a filename and loads vertices, edges, and streets
 * from the file into a new Graph structure.
 * Returns the loaded Graph.
 */
Graph* loadPlace() {
  char filename[255];

  do {
    fflush(stdin);
    printf("Informe o nome do arquivo de entrada: ");
    scanf("%254[^ \n]", filename);
    fflush(stdin);
    printf("Ficheiro \"./%s\" correcto?[S/N]: ", filename);
  } while (toupper(getchar()) == 'N');  // Repeat if the user inputs 'N'

  FILE* file = fopen(filename, "r");  // Open the file for reading
  if (!file) {
    fprintf(stderr, "Erro ao abrir o ficheiro \"%s\"\n", filename);
    return NULL;
  }

  int vertices, edges;
  // Read the number of vertices and edges from the file
  if (fscanf(file, "%d %d", &vertices, &edges) != 2) {
    puts("\n!! Ficheiro fora do formato previsto! !!\n");
    return NULL;
  }

  // Create a new graph with the given number of vertices
  Graph* graph = createGraph(vertices);

  int srcPost, destinationPost;
  float distance;

  // Read the edges from the file and add them to the graph
  while (fscanf(file, "%d %d %f", &srcPost, &destinationPost, &distance) !=
         EOF) {
    if (srcPost == 0 && destinationPost == 0) break;
    graph = addStreet(graph, srcPost, destinationPost, distance);
  }

  fclose(file);

  printf("Ficheiro \"%s\" carregado !", filename);
  return graph;
}

/**
 * Prompts the user to add a new post to the graph.
 * Calls the addPost function and updates the graph pointer accordingly.
 */
void hadleAddPost(Graph** graph) {
  puts("\n===============ADICIONAR POSTE===============\n");
  *graph = addPost(*graph);
}

/**
 * Prompts the user to remove a post from the graph.
 * Validates the input and calls the removePost function.
 */
void handleRemovePost(Graph** graph) {
  puts("\n===============REMOVER POSTE===============\n");
  int srcPost = 0;
  printf("Informe o valor do poste: ");
  // Validate the input for the post value
  validateOption(&srcPost, 1, (*graph)->totalPosts, NULL);

  *graph = removePost(*graph, srcPost);
  printf("Post %d removido com sucesso!\n", srcPost);
}

/**
 * Prompts the user to add a new street to the graph.
 * Validates input for source, destination, and distance,
 * then calls addStreet to add the street to the graph.
 */
void handleAddStreet(Graph** graph) {
  puts("\n===============ADICIONAR RUA===============\n");

  int src, dest, valid = 1;
  float distance;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, (*graph)->totalPosts, NULL);

  printf("Informe o poste de destino: ");
  validateOption(&dest, 1, (*graph)->totalPosts, NULL);

  // Validate and read the distance between the posts
  do {
    if (!valid) puts("Valor inválido!");

    printf("Informe a distancia entre os postes: ");
  } while (!(valid = scanf("%f", &distance) && distance > 0.0));

  *graph = addStreet(*graph, src, dest, distance);
  printf("Rua de %d à %d distando %.2fm adicionada !", src, dest, distance);
}

/**
 * Prompts the user to remove a street from the graph.
 * Validates input for source and destination posts,
 * then calls removeStreet to remove the street from the graph.
 */
void handleRemoveStreet(Graph** graph) {
  puts("\n===============REMOVER RUA===============\n");

  int src, dest;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, (*graph)->totalPosts, NULL);

  printf("Informe o poste de destino: ");
  validateOption(&dest, 1, (*graph)->totalPosts, NULL);

  *graph = removeStreet(*graph, src, dest);
  printf("Rua de %d à %d removida !", src, dest);
}

/**
 * Prompts the user to update the distance of a street in the graph.
 * Validates input for source, destination, and new distance,
 * then calls updateDistance to update the street's distance in the graph.
 */
void handleUpdateDistance(Graph** graph) {
  puts("\n===============EDITAR DISTÂNCIA===============\n");

  int src, dest, valid = 1;
  float newDistance;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, (*graph)->totalPosts, NULL);

  printf("Informe o poste de destino: ");
  validateOption(&dest, 1, (*graph)->totalPosts, NULL);

  // Validate and read the new distance between the posts
  do {
    if (!valid) puts("Valor inválido!");

    printf("Informe a distancia entre os postes: ");
  } while (!(valid = scanf("%f", &newDistance) && newDistance > 0.0));

  *graph = updateDistance(*graph, src, dest, newDistance);
  printf("\n\n");
}

/**
 * Handles the calculation of total illumination cost for the graph.
 * Calls illuminateStreets to calculate total illumination cost and prints it.
 */
void handleTotalIlumination(Graph* graph) {
  puts("\n===============ILLUMINAÇÃO TOTAL===============\n");

  float distance = illuminateStreets(graph, false);
  puts("\n");
  printf("Custo total: %.2fKzs", distance * COST_PER_METER);
}

/**
 * Handles the illumination of adjacent streets from a specific post in the
 * graph. Prompts the user for a post, then calls illuminateStreetsFromPost to
 * calculate and print the total illumination cost for adjacent streets.
 */
void handleIlluminateAdjacentStreet(Graph* graph) {
  puts("\n===============ILUMINAÇÃO DE RUAS ADJACENTES===============\n");

  int src;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, graph->totalPosts, NULL);

  printf("Illuminação de todas as ruas partindo do poste %d\n", src);
  float distance = illuminateStreetsFromPost(graph, src, false, true);
  puts("\n");
  printf("CUSTO TOTAL: %.2fKzs\n", distance * COST_PER_METER);
}

/**
 * Handles the calculation of the best solution for illumination in the graph.
 * Calls illuminateStreets with the efficient option enabled,
 * then calculates and prints the total cost and savings percentage.
 */
void handleBestSolution(Graph* graph) {
  puts("\n===============MELHOR SOLUÇÃO PARA A ILUMINAÇÃO===============\n");

  float totalCost = illuminateStreets(graph, true) * COST_PER_METER;
  float bestCost =
      primMST(graph) *
      COST_PER_METER;  // Calculate the best cost using Prim's algorithm
  float saved = totalCost - bestCost;

  printf(
      "\n\nCusto total: %.2fKzs\nCusto eficiente: "
      "%.2fKzs\nValor poupado: %.2fKzs (%.1f%%)\n",
      totalCost, bestCost, saved, (saved / totalCost) * 100);
}

/**
 * Handles the calculation of the best connection between two posts in the
 * graph. Prompts the user for source and destination posts, then calls
 * dijkstraSP to calculate and print the shortest path distance and cost between
 * them.
 */
void handleBestConnection(Graph* graph) {
  puts("\n===============MELHOR LIGAÇÃO ENTRE POSTES===============\n");

  int src, dest;

  printf("Informe o poste de origem: ");
  validateOption(&src, 1, graph->totalPosts, NULL);

  printf("Informe o poste de destino: ");
  validateOption(&dest, 1, graph->totalPosts, NULL);

  // Calculate the shortest path using Dijkstra's algorithm
  float distance = dijkstraSP(graph, src, dest);

  printf("\nCusto total de %d à %d distando %.2fm: %.2fKzs\n", src, dest,
         distance, distance * COST_PER_METER);
}

/**
 * Handles the destruction of the simulation by destroying the graph.
 * Prints a success message if the graph is successfully destroyed,
 * otherwise prints an error message and exits with failure.
 */
void handleDestroySimulation(Graph** graph) {
  puts("=============DESTRUIR SIMULAÇÃO============");
  destroyGraph(graph);
  if (*graph)
    perror("Ocorreu um erro ao destruir a simulação!"), exit(EXIT_FAILURE);

  puts("Simulação destruida com sucesso!");
}

/**
 * Displays the simulation menu options and prompts the user to choose an
 * option. Returns the chosen option index.
 */
int simulation_menu() {
  puts("\n===============GERIR SIMULAÇÃO===============\n");

  const char* options[] = {
      "Carregar localidade", "Adicionar poste",
      "Remover poste",       "Adicionar rua",
      "Remover rua",         "Actualizar distância entre postes",
      "Destruir simulação",  "Voltar"};

  return showOptions(options, sizeof(options) / sizeof(options[0]));
}

/**
 * Handles the overall simulation process.
 * Displays the simulation menu and performs corresponding actions based on user
 * input, including loading a graph, managing posts and streets, and calculating
 * illumination solutions.
 */
void handleSimulation(Graph** graph) {
  bool out = false;
  while (!out) {
    int option = menu(simulation_menu);
    if (!(*graph) && !(option == 1 || option == 8)) {
      // Force the option to load the graph if it is not loaded
      puts("!! Considere carregar o grafo antes de qualquer operação !!\n");
      option = 1;
    }

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
    if (!out) pause();  // Pause after each operation
  }
}

/**
 * Handles the main simulation use case.
 * Displays the main simulation menu and prompts the user to choose an option,
 * then performs corresponding actions based on user input,
 * including managing simulation tasks and calculating illumination solutions.
 * Updates the main graph pointer accordingly.
 * Returns false to indicate completion of simulation.
 */
bool simulationUseCase(Graph** mainGraph, User* user) {
  Graph* graph = *mainGraph;
  while (true) {
    clearConsole();
    puts("\n===============SIMULAÇÃO===============\n");
    int i = 1, option;

    if (!graph) puts("!! Grafo Inexistente !!");

    if (user->isAdmin) printf("%d - Gerir Simulação\n", i++);
    if (graph) {
      printf("%d - Melhor ligação entre 2 postes\n", i++);
      printf("%d - Iluminação da localidade\n", i++);
      printf("%d - Melhor solução de iluminação da localidade\n", i++);
      printf("%d - Iluminação de ruas adjacentes\n", i++);
    }

    printf("%d - Encerrar sessão\n", i);

    validateOption(&option, 1, i, NULL);

    clearConsole();
    if (option == 1 && user->isAdmin) {
      handleSimulation(&graph);
    } else if ((option == 1 || (option == 2 && user->isAdmin)) && graph) {
      handleBestConnection(graph);
    } else if ((option == 2 || (option == 3 && user->isAdmin)) && graph) {
      handleTotalIlumination(graph);
    } else if ((option == 3 || (option == 4 && user->isAdmin)) && graph) {
      handleBestSolution(graph);
    } else if ((option == 4 || (option == 5 && user->isAdmin)) && graph) {
      handleIlluminateAdjacentStreet(graph);
    } else {
      break;
    }

    if (option != 1 || (option != 1 && user->isAdmin))
      pause();  // Pause after each operation
  }

  *mainGraph = graph;
  return false;
}

#include "simulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"

Graph* loadPlace();
void hadleAddPost(Graph** graph);
void handleRemovePost(Graph** graph);
void handleAddStreet(Graph** graph);
void handleRemoveStreet(Graph** graph);
void handleUpdateDistance(Graph** graph);
void handleTotalIlumination(Graph* graph);
void handleBestSolution(Graph** graph);
void handleIlluminateAdjacentStreet(Graph** graph);
void handleDestroySimulation(Graph** graph);

void handleSimulation(Graph** graph) {
  puts("Handle Simulation");
  pause();
}

bool simulationUseCase(User* user) {
  Graph* graph = NULL;

  int i = 0, option;

  if (!graph && (option == 1 && user->isAdmin)) {
    handleSimulation(&graph);
  } else if (option == 1 || (option == 2 && user->isAdmin)) {
    handleTotalIlumination(graph);
  }
}
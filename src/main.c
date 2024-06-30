#include <paths.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tads/graph/graph.h"
#include "tads/user/user.h"
#include "usecases/auth/auth.h"
#include "usecases/simulation/simulation.h"
#include "utils/utils.h"

int main() {
  Avl* tree = loadUsers(NULL);
  User* loggedUser = NULL;
  Graph* graph = NULL;

auth: {
  loggedUser = authUseCase(&tree);
  if (!loggedUser) goto end;

  if (usersUseCase(&tree, loggedUser) == 0) {
    loggedUser = NULL;
    goto auth;
  }
}

  bool endApplication = simulationUseCase(&graph, loggedUser);
  if (!endApplication) goto auth;

end: {
  puts("Obrigado ;)\nBy: Grupo 2");
  puts("Matateu André - 20212549🤓");
  puts("Lukeny Silva - 20220718😎");
  puts("Kélsio Mateus -  20221473🧑‍💻");
}
  return 0;
}
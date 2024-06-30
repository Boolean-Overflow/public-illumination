#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

Street* createStreet(int destination, float distance) {
  Street* edge = (Street*)malloc(sizeof(Street));
  if (!edge) return NULL;

  edge->destination = destination;
  edge->distance = distance;
  edge->next = NULL;

  return edge;
}

Graph* createGraph(int vertices) {
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  graph->totalPosts = vertices;
  graph->posts = (Post*)malloc(vertices * sizeof(Post) + 1);
  graph->emptyPosts = 0;

  for (int i = 1; i <= vertices; ++i) graph->posts[i].head = NULL;

  return graph;
}

Graph* addStreet(Graph* graph, int src, int destination, float distance) {
  Street* edge = createStreet(destination, distance);
  edge->next = graph->posts[src].head;
  graph->posts[src].head = edge;

  if (graph->posts[src].head && graph->posts[destination].head) {
    graph->emptyPosts = 0;
  }

  edge = createStreet(src, distance);
  edge->next = graph->posts[destination].head;
  graph->posts[destination].head = edge;

  return graph;
}

Graph* addPost(Graph* graph) {
  if (graph->emptyPosts > 0) {
    fprintf(stderr, "Não pode haver mais de 1 poste sem ligação\n");
    return graph;
  }

  graph->totalPosts++;
  graph->posts = realloc(graph->posts, graph->totalPosts * sizeof(Post));
  graph->posts[graph->totalPosts].head = NULL;
  graph->emptyPosts++;

  printf("Poste %d adicionado!\n", graph->totalPosts);

  return graph;
}

Graph* removeStreet(Graph* graph, int src, int destination) {
  Street* current = graph->posts[src].head;
  Street* prev = NULL;

  while (current) {
    if (current->destination == destination) {
      if (!prev)
        graph->posts[src].head = current->next;
      else
        prev->next = current->next;

      free(current);
      break;
    }
    prev = current;
    current = current->next;
  }

  current = graph->posts[destination].head;
  prev = NULL;

  while (current) {
    if (current->destination == src) {
      if (!prev)
        graph->posts[destination].head = current->next;
      else
        prev->next = current->next;

      free(current);
      break;
    }
    prev = current;
    current = current->next;
  }

  if (graph->posts[src].head || graph->posts[destination].head)
    graph->emptyPosts++;

  return graph;
}

Graph* removePost(Graph* graph, int src) {
  for (int i = 0; i < graph->totalPosts; ++i) {
    Street* current = graph->posts[i].head;
    Street* prev = NULL;

    while (current) {
      if (current->destination == src) {
        if (!prev)
          graph->posts[i].head = current->next;
        else
          prev->next = current->next;

        free(current);
        break;
      }
      prev = current;
      current = current->next;
    }
  }

  Street* current = graph->posts[src].head;
  while (current) {
    Street* tmp = current;
    current = current->next;
    free(tmp);
  }

  graph->posts[src].head = NULL;

  if (src == graph->totalPosts)
    graph->posts = realloc(graph->posts, --graph->totalPosts * sizeof(Post));

  return graph;
}

float illuminateStreetsFromPost(Graph* graph, int srcPost) {
  float total = 0.0;
  Street* street = graph->posts[srcPost].head;
  if (!street) return total;
  printf("\n [%d]", srcPost);
  while (street) {
    if (srcPost < street->destination) total += street->distance;
    printf("-> %d(%.2fm) ", street->destination, street->distance);
    street = street->next;
  }
  printf("\n");
  return total;
}

float illuminateStreets(Graph* graph) {
  if (!graph) {
    puts("Grafo inexistente!\n Considere criar uma localidade.");
    return 0.0;
  }
  float total = 0.0;
  for (int post = 1; post <= graph->totalPosts; ++post)
    total += illuminateStreetsFromPost(graph, post);

  return total;
}

Graph* updateDistance(Graph* graph, int src, int dest, float newDistance) {
  Street* current = graph->posts[src].head;
  float oldDistance;

  while (current) {
    if (current->destination == dest) {
      oldDistance = current->distance;
      current->distance = newDistance;
      break;
    }
    current = current->next;
  }

  current = graph->posts[dest].head;
  while (current) {
    if (current->destination == src) {
      current->distance = newDistance;
      break;
    }
    current = current->next;
  }

  printf("Rua %d à %d antes %.2f agora %.2f", src, dest, oldDistance,
         newDistance);

  return graph;
}

Graph* destroyGraph(Graph* graph) {
  for (int i = 0; i <= graph->totalPosts; ++i) {
    Street* current = graph->posts[i].head;

    while (current) {
      Street* temp = current;
      current = current->next;
      free(temp);
    }
  }
  free(graph->posts);
  free(graph);

  return NULL;
}

float printMST(int parent[], float distances[], int totalPosts) {
  printf("Arestas do MST:\n");
  float cost = 0.0;
  for (int post = 2; post <= totalPosts; post++) {
    printf("%d - %d: %.2fm\n", parent[post], post, distances[post]);
    cost += distances[post];
  }

  return cost;
}

int minDistance(float dist[], int mstSet[], int totalPosts) {
  float min = 1e9;
  int min_index;

  for (int v = 1; v <= totalPosts; v++)
    if (mstSet[v] == 0 && dist[v] < min) min = dist[v], min_index = v;

  return min_index;
}

float primMST(Graph* graph) {
  int totalPosts = graph->totalPosts;
  int parent[totalPosts + 1];
  float distances[totalPosts + 1];
  int mstSet[totalPosts + 1];

  for (int i = 1; i <= totalPosts; i++) distances[i] = 1e9, mstSet[i] = 0;

  distances[1] = 0.0;
  parent[1] = -1;

  for (int count = 1; count <= totalPosts - 1; count++) {
    int post = minDistance(distances, mstSet, totalPosts);
    mstSet[post] = 1;

    Street* current = graph->posts[post].head;
    while (current) {
      int destination = current->destination;
      if (mstSet[destination] == 0 &&
          current->distance < distances[destination]) {
        parent[destination] = post;
        distances[destination] = current->distance;
      }
      current = current->next;
    }
  }

  return printMST(parent, distances, totalPosts);
}

void printPath(int parent[], int destination) {
  if (parent[destination] == -1) {
    printf("%d ", destination);
    return;
  }
  printPath(parent, parent[destination]);
  printf("%d ", destination);
}

void printShortestPaths(int parent[], float dist[], int totalPosts, int src) {
  printf("Caminhos mais curtos a partir do vértice %d:\n", src);
  for (int destination = 1; destination <= totalPosts; destination++) {
    if (destination != src) {
      printf("Caminho até %d: ", destination);
      printPath(parent, destination);
      printf(" com custo %.2f\n", dist[destination]);
    }
  }
}

float dijkstraSP(Graph* graph, int src, int destination) {
  int totalPosts = graph->totalPosts;
  float distances[totalPosts + 1];
  int visited[totalPosts + 1];
  int parent[totalPosts + 1];

  for (int i = 1; i <= totalPosts; i++) {
    distances[i] = 1e9;
    visited[i] = 0;
    parent[i] = -1;
  }

  distances[src] = 0.0;

  for (int count = 1; count <= totalPosts - 1; count++) {
    int post = minDistance(distances, visited, totalPosts);
    visited[post] = 1;

    Street* current = graph->posts[post].head;
    while (current) {
      int destination = current->destination;
      if (!visited[destination] && distances[post] != 1e9 &&
          distances[post] + current->distance < distances[destination]) {
        distances[destination] = distances[post] + current->distance;
        parent[destination] = post;
      }
      current = current->next;
    }
  }

  printPath(parent, destination);
  return distances[destination];
}

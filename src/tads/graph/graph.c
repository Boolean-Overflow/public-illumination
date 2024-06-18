#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

Street* newEdge(int destination, float distance) {
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

  // TODO: Change to base 0
  for (int i = 1; i <= vertices; ++i) graph->posts[i].head = NULL;

  return graph;
}

Graph* addStreet(Graph* graph, int src, int destination, float distance) {
  Street* edge = newEdge(destination, distance);
  edge->next = graph->posts[src].head;
  graph->posts[src].head = edge;

  edge = newEdge(src, distance);
  edge->next = graph->posts[destination].head;
  graph->posts[destination].head = edge;

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

  return graph;
}

void illuminateStreetsFromPost(Graph* graph, int srcPost) {
  Street* street = graph->posts[srcPost].head;
  printf("\n [%d]", srcPost);
  while (street) {
    printf("-> %d(%.2fm) ", street->destination, street->distance);
    street = street->next;
  }
  printf("\n");
}

void illuminateStreets(Graph* graph) {
  // TODO: Change to base 0
  if (!graph) {
    puts("Grafo inexistente!\n Considere criar uma localidade.");
    return;
  }
  for (int post = 1; post <= graph->totalPosts; ++post) illuminateStreetsFromPost(graph, post);
}

Graph* updateDistance(Graph* graph, int src, int dest, float newDistance) {
  Street* current = graph->posts[src].head;

  while (current) {
    if (current->destination == dest) {
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

  return graph;
}

Graph* destroyGraph(Graph* graph) {
  // TODO: Change to base 0
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
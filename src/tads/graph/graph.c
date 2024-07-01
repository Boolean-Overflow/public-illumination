#include "graph.h"  // Include the header file for graph structures and functions

#include <stdio.h>
#include <stdlib.h>

/**
 * Creates a new street (edge) in the graph.
 * Initializes the destination and distance of the street.
 * Returns a pointer to the created Street structure.
 */
Street* createStreet(int destination, float distance) {
  Street* edge = (Street*)malloc(sizeof(Street));
  if (!edge) return NULL;

  edge->destination = destination;
  edge->distance = distance;
  edge->next = NULL;

  return edge;
}

/**
 * Creates a new graph with the specified number of vertices (posts).
 * Initializes an array of posts, each having a head pointer to the list of
 * adjacent streets. Returns a pointer to the created Graph structure.
 */
Graph* createGraph(int vertices) {
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  graph->totalPosts = vertices;
  graph->posts = (Post*)malloc(vertices * sizeof(Post) +
                               1);  // Allocate space for posts array
  graph->emptyPosts = 0;

  for (int i = 1; i <= vertices; ++i)
    graph->posts[i].head = NULL;  // Initialize head pointers to NULL

  return graph;
}

/**
 * Adds a new street (edge) between two posts (vertices) in the graph.
 * Creates a street from src to destination and vice versa with specified
 * distance. Returns a pointer to the updated Graph structure.
 */
Graph* addStreet(Graph* graph, int src, int destination, float distance) {
  // Add street from src to destination
  Street* edge = createStreet(destination, distance);
  edge->next = graph->posts[src].head;
  graph->posts[src].head = edge;

  // Update emptyPosts count if posts have streets
  if (graph->posts[src].head && graph->posts[destination].head) {
    graph->emptyPosts = 0;
  }

  // Add street from destination to src (bidirectional)
  edge = createStreet(src, distance);
  edge->next = graph->posts[destination].head;
  graph->posts[destination].head = edge;

  return graph;
}

/**
 * Adds a new post (vertex) to the graph.
 * Increases the totalPosts count and reallocates memory for the posts array.
 * Returns a pointer to the updated Graph structure.
 */
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

/**
 * Removes a street (edge) between two posts (vertices) in the graph.
 * Removes the street from src to destination and vice versa.
 * Returns a pointer to the updated Graph structure.
 */
Graph* removeStreet(Graph* graph, int src, int destination) {
  // Remove street from src to destination
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

  // Remove street from destination to src (bidirectional)
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

  // Update emptyPosts count if posts have no streets
  if (!(graph->posts[src].head || graph->posts[destination].head))
    graph->emptyPosts++;

  return graph;
}

/**
 * Removes a post (vertex) and all associated streets (edges) from the graph.
 * Removes all streets connected to the specified post and deallocates memory.
 * Returns a pointer to the updated Graph structure.
 */
Graph* removePost(Graph* graph, int src) {
  // Remove all streets connected to the specified post (src)
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

  // Free memory for streets connected to src post
  Street* current = graph->posts[src].head;
  while (current) {
    Street* tmp = current;
    current = current->next;
    free(tmp);
  }

  graph->posts[src].head = NULL;

  // If src was the last post, reallocate memory for posts array
  if (src == graph->totalPosts)
    graph->posts = realloc(graph->posts, --graph->totalPosts * sizeof(Post));

  return graph;
}

/**
 * Calculates the total cost of illuminating streets adjacent to a specific post
 * (vertex). Optionally calculates only (prints) or sums all street distances.
 * Returns the total illumination cost as a float.
 */
float illuminateStreetsFromPost(Graph* graph, int srcPost, bool calculateOnly,
                                bool sumAll) {
  float total = 0.0;
  float repeated = 0.0;
  Street* street = graph->posts[srcPost].head;
  if (!street) return total;
  if (!calculateOnly) printf("\n [%d]", srcPost);
  while (street) {
    if (srcPost > street->destination) repeated += street->distance;
    total += street->distance;

    if (!calculateOnly)
      printf("-> %d(%.2fm) ", street->destination, street->distance);

    street = street->next;
  }
  if (!calculateOnly) printf("\n");

  return sumAll ? total : total - repeated;
}

/**
 * Calculates the total cost of illuminating all streets in the graph.
 * Optionally calculates only (prints) or sums all street distances.
 * Returns the total illumination cost as a float.
 */
float illuminateStreets(Graph* graph, bool calculateOnly) {
  if (!graph) {
    puts("Grafo inexistente!\n Considere criar uma localidade.");
    return 0.0;
  }

  float total = 0.0;
  for (int post = 1; post <= graph->totalPosts; ++post)
    total += illuminateStreetsFromPost(graph, post, calculateOnly, false);

  return total;
}

/**
 * Updates the distance of a street (edge) between two posts (vertices) in the
 * graph. Updates the distance for the street from src to destination and vice
 * versa. Returns a pointer to the updated Graph structure.
 */
Graph* updateDistance(Graph* graph, int src, int dest, float newDistance) {
  Street* current = graph->posts[src].head;
  float oldDistance = -1.0;

  // Update distance for street from src to destination
  while (current) {
    if (current->destination == dest) {
      oldDistance = current->distance;
      current->distance = newDistance;
      break;
    }
    current = current->next;
  }

  // Update distance for street from destination to src (bidirectional)
  current = graph->posts[dest].head;
  while (current) {
    if (current->destination == src) {
      current->distance = newDistance;
      break;
    }
    current = current->next;
  }

  if (oldDistance == -1.0) {
    printf("Sem conexão entre os postes %d e %d\n", src, dest);
    return graph;
  }

  printf("Rua %d à %d antes %.2f agora %.2f\n", src, dest, oldDistance,
         newDistance);

  return graph;
}

/**
 * Destroys the graph and deallocates all allocated memory.
 * Frees memory for all streets and the posts array.
 */
void destroyGraph(Graph** graph) {
  for (int i = 0; i <= (*graph)->totalPosts; ++i) {
    Street* current = (*graph)->posts[i].head;

    while (current) {
      Street* temp = current;
      current = current->next;
      free(temp);
    }
  }
  free((*graph)->posts);
  free(*graph);

  *graph = NULL;
}

/**
 * Helper function to find the vertex with minimum distance value,
 * from the set of vertices not yet included in the minimum spanning tree (MST).
 * Returns the index of the minimum distance vertex.
 */
int minDistance(float dist[], int mstSet[], int totalPosts) {
  float min = 1e9;
  int min_index;

  for (int v = 1; v <= totalPosts; v++)
    if (mstSet[v] == 0 && dist[v] < min) min = dist[v], min_index = v;

  return min_index;
}

/**
 * Prints the minimum spanning tree (MST) edges.
 * Prints each edge from parent array along with its distance and returns the
 * total cost.
 */
float printMST(int parent[], float distances[], int totalPosts) {
  printf("Ruas a serem iluminadas:\n");
  float cost = 0.0;
  for (int post = 2; post <= totalPosts; post++) {
    printf("%d -> %d: %.2fm\n", parent[post], post, distances[post]);
    cost += distances[post];
  }

  return cost;
}

/**
 * Prints the shortest path from source to destination using parent array.
 */
void printPath(int parent[], int destination) {
  if (parent[destination] == -1) {
    printf("%d ", destination);
    return;
  }
  printPath(parent, parent[destination]);
  printf(" -> %d", destination);
}

/**
 * Prints the minimum spanning tree (MST) of the graph using Prim's algorithm.
 * Prints each street in the MST and returns the total cost of the MST.
 */
float primMST(Graph* graph) {
  int totalPosts = graph->totalPosts;
  int parent[totalPosts + 1];
  float distances[totalPosts + 1];
  int mstSet[totalPosts + 1];

  // Initialize distances and mstSet arrays
  for (int i = 1; i <= totalPosts; i++) distances[i] = 1e9, mstSet[i] = 0;

  distances[1] = 0.0;
  parent[1] = -1;

  // Construct MST using Prim's algorithm
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

  // Print MST and return total cost
  return printMST(parent, distances, totalPosts);
}

/**
 * Calculates the shortest path from a source vertex to a destination vertex
 * using Dijkstra's algorithm. Returns the shortest path cost as a float.
 */
float dijkstraSP(Graph* graph, int src, int destination) {
  int totalPosts = graph->totalPosts;
  float distances[totalPosts + 1];
  int visited[totalPosts + 1];
  int parent[totalPosts + 1];

  // Initialize distances, visited, and parent arrays
  for (int i = 1; i <= totalPosts; i++) {
    distances[i] = 1e9;
    visited[i] = 0;
    parent[i] = -1;
  }

  distances[src] = 0.0;

  // Compute shortest paths using Dijkstra's algorithm
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

  // Print shortest path from src to destination
  printPath(parent, destination);

  // Return shortest path cost
  return distances[destination];
}

#ifndef __GRAPH_ADJACENCY_LIST_
#define __GRAPH_ADJACENCY_LIST_

typedef struct Street {
  int destination;
  float distance;
  struct Street* next;
} Street;

typedef struct Post {
  Street* head;
} Post;

typedef struct Graph {
  int totalPosts;
  Post* posts;
} Graph;

Graph* createGraph(int posts);
Graph* addStreet(Graph* graph, int srcPost, int destinationPost, float distance);
Graph* updateDistance(Graph* graph, int srcPost, int destinationPost, float newDistance);
Graph* removePost(Graph* graph, int srcPost);
void illuminateStreetsFromPost(Graph* graph, int srcPost);
void illuminateStreets(Graph* graph);
Graph* destroyGraph(Graph* graph);

#endif
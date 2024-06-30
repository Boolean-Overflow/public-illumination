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
  int emptyPosts;
  Post* posts;
} Graph;

Graph* createGraph(int posts);
Graph* addStreet(Graph* graph, int srcPost, int destinationPost,
                 float distance);
Graph* removeStreet(Graph* graph, int src, int destination);
Graph* updateDistance(Graph* graph, int srcPost, int destinationPost,
                      float newDistance);
Graph* addPost(Graph* graph);
Graph* removePost(Graph* graph, int srcPost);
float illuminateStreetsFromPost(Graph* graph, int srcPost);
float illuminateStreets(Graph* graph);
Graph* destroyGraph(Graph* graph);
float primMST(Graph* graph);
float dijkstraSP(Graph* graph, int src, int destination);

#endif
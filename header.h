#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// ----------------- Visualizer ----------------------------
#define MAX_SIZE 100
#define INF INT_MAX

typedef struct
{
    int x, y;
} Node;

void initGrid();
bool isValid(int x, int y);
void dijkstra_visual();
void bfs_visual();
void dfs_visual();
void printPath();
void print_path(int *predecessors, int start_node, int end_node);

// ------------------- Graph Structures -------------------
typedef struct mygraph graph;

typedef struct mygraph {
    int numnodes;
    bool **edges;
} graph;

// For Minimum Spanning Tree (Prim's Algorithm)
typedef struct {
    int from;
    int to;
    int weight;
} edge;


// ------------------- Graph Function Declarations -------------------
graph *create_graph(int numnodes);
void destroy_graph(graph *g);
void print_graph(graph *g);
bool add_edge(graph *g, unsigned int from_node, unsigned int to_node);
bool has_edge(graph *g, unsigned int from_node, unsigned int to_node);

void bfs(graph *g, int start_node);
void dfs(graph *g, int start_node);
bool is_cyclic(graph *g);
int *shortest_path_dijkstra(graph *g, int start_node, int end_node, int **predecessors);
graph *transpose_graph(graph *g);

//add in v1-version
bool remove_edge(graph *g, unsigned int from_node, unsigned int to_node);
int get_in_degree(graph *g, unsigned int node);
int get_out_degree(graph *g, unsigned int node);

edge *get_minimum_spanning_tree(graph *g);

bool has_path(graph *g, int start, int end);

bool is_connected(graph *g);

graph* clone_graph(graph *g);


//########################## Menu Functions start from here #################################
void show_graph_menu();



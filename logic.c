#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<assert.h>
#include<limits.h>
#include "header.h"

// Create a new graph with 'numnodes' nodes
graph *create_graph(int numnodes) {
    graph *g = malloc(sizeof(*g));
    if (g == NULL) {
        return NULL;
    }
    g->numnodes = numnodes;

    // Allocate memory for the edges matrix
    g->edges = calloc(g->numnodes, sizeof(bool *));
    if (g->edges == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < g->numnodes; i++) {
        g->edges[i] = calloc(g->numnodes, sizeof(bool));
        if (g->edges[i] == NULL) {
            destroy_graph(g);
            return NULL;
        }
    }

    return g;
}

// Destroy a graph and free all allocated memory
void destroy_graph(graph* g) {
    if (g != NULL) {
        if (g->edges != NULL) {
            for (int i = 0; i < g->numnodes; i++) {
                free(g->edges[i]);
            }
            free(g->edges);
        }
        free(g);
    }
}

// Print the graph in DOT format
void print_graph(graph *g) {
    printf("Digraph {\n");
    for (int from = 0; from < g->numnodes; from++) {
        for (int to = 0; to < g->numnodes; to++) {
            if (g->edges[from][to]) {
                printf("%d -> %d;\n", from, to);
            }
        }
    }
    printf("}\n");
}

// Add an edge to the graph
bool add_edge(graph *g, unsigned int from_node, unsigned int to_node) {
    assert(g != NULL);
    assert(from_node < g->numnodes);
    assert(to_node < g->numnodes);

    if (has_edge(g, from_node, to_node)) {
        return false;
    }
    g->edges[from_node][to_node] = true;
    return true;
}

// Check if an edge exists between two nodes
bool has_edge(graph *g, unsigned int from_node, unsigned int to_node) {
    assert(g != NULL);
    assert(from_node < g->numnodes);
    assert(to_node < g->numnodes);
    
    return g->edges[from_node][to_node];
}

// Helper function for DFS
static void dfs_helper(graph *g, int node, bool *visited) {
    visited[node] = true;
    printf("%d ", node);

    for (int i = 0; i < g->numnodes; i++) {
        // if (g->edges[node][i] && !visited[i]) {
        //     dfs_helper(g, i, visited);
        // }
        if ((g->edges[node][i] || g->edges[i][node]) && !visited[i]) {
            dfs_helper(g, i, visited);  // Recur for the unvisited node
        }
    }
}

// Perform Depth-First Search (DFS)
void dfs(graph *g, int start_node) {
    bool *visited = calloc(g->numnodes, sizeof(bool));
    if (visited == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    dfs_helper(g, start_node, visited);
    printf("\n");
    free(visited);
}

// Perform Breadth-First Search (BFS)
void bfs(graph *g, int start_node) {
    bool *visited = calloc(g->numnodes, sizeof(bool));
    int *queue = malloc(g->numnodes * sizeof(int));
    if (visited == NULL || queue == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    int front = 0, rear = 0;
    visited[start_node] = true;
    queue[rear++] = start_node;

    while (front < rear) {
        int node = queue[front++];
        printf("%d ", node);

        for (int i = 0; i < g->numnodes; i++) {
            // if (g->edges[node][i] && !visited[i]) {
            //     visited[i] = true;
            //     queue[rear++] = i;
            // }
            if ((g->edges[node][i] || g->edges[i][node]) && !visited[i]) {
                visited[i] = true;  // Mark the node as visited
                queue[rear++] = i;   // Enqueue the unvisited node
            }
        }
    }
    printf("\n");
    free(visited);
    free(queue);
}

// Helper function to detect a cycle in the graph
bool is_cyclic_helper(graph *g, int node, bool *visited, bool *rec_stack) {
    // If the node is currently in the recursion stack, we found a cycle
    if (rec_stack[node]) {
        return true;
    }

    // If the node is already visited, there's no cycle from here
    if (visited[node]) {
        return false;
    }

    // Mark the node as visited and add it to the recursion stack
    visited[node] = true;
    rec_stack[node] = true;

    // Visit all the neighbors of the current node
    for (int i = 0; i < g->numnodes; i++) {
        // Check if there's an edge and recurse if the neighbor has not been visited
        if (g->edges[node][i] != 0 && is_cyclic_helper(g, i, visited, rec_stack)) {
            return true;
        }
    }

    // Remove the node from the recursion stack
    rec_stack[node] = false;
    return false;
}

// Check if the graph has a cycle
bool is_cyclic(graph *g) {
    // Allocate visited and rec_stack arrays
    bool *visited = calloc(g->numnodes, sizeof(bool));
    bool *rec_stack = calloc(g->numnodes, sizeof(bool));

    if (visited == NULL || rec_stack == NULL) {
        printf("Memory allocation failed\n");
        return false;
    }

    // Check for cycles in all unvisited nodes
    for (int i = 0; i < g->numnodes; i++) {
        if (!visited[i]) {
            if (is_cyclic_helper(g, i, visited, rec_stack)) {
                free(visited);
                free(rec_stack);
                return true;
            }
        }
    }

    // No cycle found
    free(visited);
    free(rec_stack);
    return false;
}

// Dijkstra's algorithm to find the shortest paths and store paths
#define INF INT_MAX

int *shortest_path_dijkstra(graph *g, int start_node, int end_node, int **predecessors) {
    int *distances = malloc(g->numnodes * sizeof(int));
    *predecessors = malloc(g->numnodes * sizeof(int));
    bool *visited = calloc(g->numnodes, sizeof(bool));
    if (distances == NULL || visited == NULL || *predecessors == NULL) {
        printf("Memory allocation failed\n");
        free(distances);
        free(*predecessors);
        free(visited);
        return NULL;
    }

    // Initialize distances and predecessors
    for (int i = 0; i < g->numnodes; i++) {
        distances[i] = INF;
        (*predecessors)[i] = -1;  // Initialize predecessors to -1 (no path)
    }
    distances[start_node] = 0;

    for (int i = 0; i < g->numnodes; i++) {
        // Find the unvisited node with the smallest distance
        int min_dist = INF;
        int u = -1;
        for (int j = 0; j < g->numnodes; j++) {
            if (!visited[j] && distances[j] < min_dist) {
                min_dist = distances[j];
                u = j;
            }
        }
        if (u == -1) break; // No more reachable nodes

        visited[u] = true;

        // Early exit if we reached the end node
        if (u == end_node) break;

        // Relax edges
        for (int v = 0; v < g->numnodes; v++) {
            if (g->edges[u][v] && !visited[v] && distances[u] != INF &&
                distances[u] + g->edges[u][v] < distances[v]) {
                distances[v] = distances[u] + g->edges[u][v];
                (*predecessors)[v] = u;  // Update predecessor
            }
        }
    }

    free(visited);
    return distances;
}

// Function to print the path
void print_path(int *predecessors, int start_node, int end_node) {
    if (end_node == start_node) {
        printf("%d ", start_node);
        return;
    }
    if (predecessors[end_node] == -1) {
        printf("No path\n");
        return;
    }
    print_path(predecessors, start_node, predecessors[end_node]);
    printf("%d ", end_node);
}


// Transpose a graph
graph *transpose_graph(graph *g) {
    graph *transposed = create_graph(g->numnodes);
    if (transposed == NULL) {
        return NULL;
    }

    for (int i = 0; i < g->numnodes; i++) {
        for (int j = 0; j < g->numnodes; j++) {
            if (g->edges[i][j]) {
                add_edge(transposed, j, i);
            }
        }
    }

    return transposed;
}

// Remove an edge from the graph
bool remove_edge(graph *g, unsigned int from_node, unsigned int to_node) {
    assert(g != NULL);
    assert(from_node < g->numnodes);
    assert(to_node < g->numnodes);

    if (!has_edge(g, from_node, to_node)) {
        return false;
    }
    g->edges[from_node][to_node] = false;
    return true;
}

int get_in_degree(graph *g, unsigned int node) {
    assert(g != NULL);
    assert(node < g->numnodes);
    
    int count = 0;
    for(int i = 0; i < g->numnodes; i++) {
        if(g->edges[i][node]) {
            count++;
        }
    }
    return count;
}

int get_out_degree(graph *g, unsigned int node) {
    assert(g != NULL);
    assert(node < g->numnodes);
    
    int count = 0;
    for(int i = 0; i < g->numnodes; i++) {
        if(g->edges[node][i]) {
            count++;
        }
    }
    return count;
}

// Minimum Spanning Tree (Prim's Algorithm) - For weighted graphs

edge* get_minimum_spanning_tree(graph *g) {
    bool *visited = calloc(g->numnodes, sizeof(bool));
    edge *mst = malloc((g->numnodes - 1) * sizeof(edge));
    int *key = malloc(g->numnodes * sizeof(int));
    int *parent = malloc(g->numnodes * sizeof(int));
    
    if(!visited || !mst || !key || !parent) {
        free(visited);
        free(mst);
        free(key);
        free(parent);
        return NULL;
    }
    
    // Initialize keys and parent
    for(int i = 0; i < g->numnodes; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }
    
    key[0] = 0;  // Start with vertex 0
    
    for(int count = 0; count < g->numnodes - 1; count++) {
        // Find vertex with minimum key
        int min_key = INT_MAX;
        int min_index = -1;
        
        for(int v = 0; v < g->numnodes; v++) {
            if(!visited[v] && key[v] < min_key) {
                min_key = key[v];
                min_index = v;
            }
        }
        
        visited[min_index] = true;
        
        // Update key values of adjacent vertices
        for(int v = 0; v < g->numnodes; v++) {
            if(g->edges[min_index][v] && !visited[v] && 1 < key[v]) {
                parent[v] = min_index;
                key[v] = 1;  // Using weight 1 for unweighted graph
            }
        }
    }
    
    // Construct MST edges
    for(int i = 1; i < g->numnodes; i++) {
        mst[i-1].from = parent[i];
        mst[i-1].to = i;
        mst[i-1].weight = 1;  // Using weight 1 for unweighted graph
    }
    
    free(visited);
    free(key);
    free(parent);
    return mst;
}

// Graph properties checkers
bool is_connected(graph *g) {
    bool *visited = calloc(g->numnodes, sizeof(bool));
    if(!visited) return false;
    
    // Start DFS from vertex 0
    dfs_helper(g, 0, visited);
    
    // Check if all vertices were visited
    for(int i = 0; i < g->numnodes; i++) {
        if(!visited[i]) {
            free(visited);
            return false;
        }
    }
    
    free(visited);
    return true;
}

graph* clone_graph(graph *g) {
    if(!g) return NULL;
    
    graph *clone = create_graph(g->numnodes);
    if(!clone) return NULL;
    
    // Copy all edges
    for(int i = 0; i < g->numnodes; i++) {
        for(int j = 0; j < g->numnodes; j++) {
            clone->edges[i][j] = g->edges[i][j];
        }
    }
    
    return clone;
}

// -------------------- Visualization --------------------------

int rows, cols;
char grid[MAX_SIZE][MAX_SIZE];
int dist[MAX_SIZE][MAX_SIZE];
Node prev[MAX_SIZE][MAX_SIZE];
Node start, end;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

// Queue for BFS
Node queue[MAX_SIZE * MAX_SIZE];
int front = 0, rear = 0;

// Stack for DFS
Node stack[MAX_SIZE * MAX_SIZE];
int top = -1;

void initGrid()
{
    printf("Enter grid dimensions (rows cols): ");
    scanf("%d %d", &rows, &cols);

    printf("Enter grid (use 'S' for start, 'E' for end, '#' for blocks, '.' for empty):\n");
    for (int i = 0; i < rows; i++)
    {
        scanf("%s", grid[i]);
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] == 'S')
            {
                start = (Node){i, j};
            }
            else if (grid[i][j] == 'E')
            {
                end = (Node){i, j};
            }
        }
    }
}

bool isValid(int x, int y)
{
    return x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] != '#';
}

void dijkstra_visual()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dist[i][j] = INF;
            prev[i][j] = (Node){-1, -1};
        }
    }

    dist[start.x][start.y] = 0;
    Node pq[MAX_SIZE * MAX_SIZE];
    int pqSize = 0;

    pq[pqSize++] = start;

    while (pqSize > 0)
    {
        int minIndex = 0;
        for (int i = 1; i < pqSize; i++)
        {
            if (dist[pq[i].x][pq[i].y] < dist[pq[minIndex].x][pq[minIndex].y])
            {
                minIndex = i;
            }
        }

        Node current = pq[minIndex];
        pq[minIndex] = pq[--pqSize];

        if (current.x == end.x && current.y == end.y)
        {
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (isValid(nx, ny))
            {
                int alt = dist[current.x][current.y] + 1;
                if (alt < dist[nx][ny])
                {
                    dist[nx][ny] = alt;
                    prev[nx][ny] = current;
                    pq[pqSize++] = (Node){nx, ny};
                }
            }
        }
    }
}

void bfs_visual()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dist[i][j] = INF;
            prev[i][j] = (Node){-1, -1};
        }
    }

    front = rear = 0;
    queue[rear++] = start;
    dist[start.x][start.y] = 0;

    while (front < rear)
    {
        Node current = queue[front++];

        if (current.x == end.x && current.y == end.y)
        {
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (isValid(nx, ny) && dist[nx][ny] == INF)
            {
                dist[nx][ny] = dist[current.x][current.y] + 1;
                prev[nx][ny] = current;
                queue[rear++] = (Node){nx, ny};
            }
        }
    }
}

void dfs_visual()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dist[i][j] = INF;
            prev[i][j] = (Node){-1, -1};
        }
    }

    top = -1;
    stack[++top] = start;
    dist[start.x][start.y] = 0;

    while (top >= 0)
    {
        Node current = stack[top--];

        if (current.x == end.x && current.y == end.y)
        {
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (isValid(nx, ny) && dist[nx][ny] == INF)
            {
                dist[nx][ny] = dist[current.x][current.y] + 1;
                prev[nx][ny] = current;
                stack[++top] = (Node){nx, ny};
            }
        }
    }
}

void printPath() {
    // Create a temporary copy of the grid to display the path
    char tempGrid[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            tempGrid[i][j] = grid[i][j];
        }
    }

    // Mark the shortest path on the temporary grid
    Node current = end;
    while (current.x != -1 && current.y != -1) {
        if (tempGrid[current.x][current.y] != 'S' && tempGrid[current.x][current.y] != 'E') {
            tempGrid[current.x][current.y] = '*';
        }
        current = prev[current.x][current.y];
    }

    // Print the temporary grid
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", tempGrid[i][j]);
        }
        printf("\n");
    }
}
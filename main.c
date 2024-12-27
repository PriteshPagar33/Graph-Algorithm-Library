#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "header.h"

// Function to display the main menu
void show_main_menu()
{
    printf("\n========== Main Menu ==========\n");
    printf("1. Graph Operations\n");
    printf("2. Shortest Path Visualizer\n");
    printf("3. Friends Recommendation System\n");
    printf("0. Exit\n");
    printf("===============================\n");
    printf("Enter your choice: ");
}

// Function to display the graph operations menu
void show_graph_menu()
{
    printf("\n========== Graph Menu ==========\n");
    printf("1. Add Edge\n");
    printf("2. Print Graph\n");
    printf("3. Perform DFS\n");
    printf("4. Perform BFS\n");
    printf("5. Check if Graph is Cyclic\n");
    printf("6. Find Shortest Path (Dijkstra)\n");
    printf("7. Transpose Graph\n");
    printf("8. Check In-degree and Out-degree of Node\n");
    printf("9. Check if Graph is Connected\n");
    printf("0. Back to Main Menu\n");
    printf("===============================\n");
    printf("Enter your choice: ");
}

// Function to recommend friends for a user
void recommend_friends(graph *g, int user)
{
    printf("\n===== Friend Recommendations for User %d =====\n", user);

    if (user < 0 || user >= g->numnodes)
    {
        printf("Invalid user ID. Please try again.\n");
        return;
    }

    int *distances = NULL;
    int *predecessors = NULL;

    // Get shortest paths from the user to all other nodes
    distances = shortest_path_dijkstra(g, user, -1, &predecessors);

    if (!distances)
    {
        printf("Failed to compute shortest paths.\n");
        return;
    }

    // Recommend friends
    int recommendations = 0; // Count of friend recommendations
    for (int i = 0; i < g->numnodes; i++)
    {
        if (i == user)
            continue; // Skip the user themselves

        // Check if not directly connected and reachable
        if (!has_edge(g, user, i) && distances[i] != INF)
        {
            printf("Recommend User %d\n", i);
            recommendations++;
        }
    }

    if (recommendations == 0)
    {
        printf("No friend recommendations available for User %d.\n", user);
    }

    free(distances);
    free(predecessors);
}

int main()
{
    // Variables for graph operations
    graph *g = NULL;
    int graphNodes = 0;

    printf("Enter the number of nodes in the graph: ");
    scanf("%d", &graphNodes);
    g = create_graph(graphNodes);
    if (!g)
    {
        printf("Failed to create graph. Exiting...\n");
        return 1;
    }

    // General variables
    int main_choice, graph_choice, algorithm_choice;
    int fromNode, toNode, startNode, endNode;

    do
    {
        show_main_menu();
        scanf("%d", &main_choice);

        switch (main_choice)
        {
        case 1: // Graph Operations
            do
            {
                show_graph_menu();
                scanf("%d", &graph_choice);
                switch (graph_choice)
                {
                case 1: // Add Edge
                    printf("Enter the source node: ");
                    scanf("%d", &fromNode);
                    printf("Enter the destination node: ");
                    scanf("%d", &toNode);
                    if (fromNode < 0 || fromNode >= graphNodes || toNode < 0 || toNode >= graphNodes)
                    {
                        printf("Invalid node index. Try again.\n");
                        break;
                    }
                    if (add_edge(g, fromNode, toNode))
                    {
                        printf("Edge added successfully.\n");
                    }
                    else
                    {
                        printf("Failed to add edge. Ensure nodes are valid and edge doesn't already exist.\n");
                    }
                    break;

                case 2: // Print Graph
                    print_graph(g);
                    break;

                case 3: // Perform DFS
                    printf("Enter the starting node for DFS: ");
                    scanf("%d", &startNode);
                    if (startNode < 0 || startNode >= graphNodes)
                    {
                        printf("Invalid starting node.\n");
                        break;
                    }
                    printf("DFS from node %d:\n", startNode);
                    dfs(g, startNode);
                    break;

                case 4: // Perform BFS
                    printf("Enter the starting node for BFS: ");
                    scanf("%d", &startNode);
                    if (startNode < 0 || startNode >= graphNodes)
                    {
                        printf("Invalid starting node.\n");
                        break;
                    }
                    printf("BFS from node %d:\n", startNode);
                    bfs(g, startNode);
                    break;

                case 5: // Check if Graph is Cyclic
                    printf("Is the graph cyclic? %s\n", is_cyclic(g) ? "Yes" : "No");
                    break;

                case 6: // Shortest Path (Dijkstra)
                    printf("Enter the starting node for Dijkstra's algorithm: ");
                    scanf("%d", &startNode);
                    printf("Enter the ending node for Dijkstra's algorithm: ");
                    scanf("%d", &endNode);

                    if (startNode < 0 || startNode >= graphNodes || endNode < 0 || endNode >= graphNodes)
                    {
                        printf("Invalid node(s). Please enter valid indices.\n");
                        break;
                    }

                    int *predecessors = NULL;
                    int *distances = shortest_path_dijkstra(g, startNode, endNode, &predecessors);
                    if (distances)
                    {
                        if (distances[endNode] == INF)
                        {
                            printf("No path exists from %d to %d.\n", startNode, endNode);
                        }
                        else
                        {
                            printf("Shortest distance from %d to %d: %d\n", startNode, endNode, distances[endNode]);
                            printf("Path: ");
                            print_path(predecessors, startNode, endNode);
                            printf("\n");
                        }
                        free(distances);
                        free(predecessors);
                    }
                    else
                    {
                        printf("Failed to compute shortest path.\n");
                    }
                    break;

                case 7: // Transpose Graph
                    printf("Transposed Graph:\n");
                    graph *transposed = transpose_graph(g);
                    if (transposed)
                    {
                        print_graph(transposed);
                        destroy_graph(transposed);
                    }
                    else
                    {
                        printf("Failed to transpose graph.\n");
                    }
                    break;

                case 8: // Check In-degree and Out-degree of Node
                    printf("Enter the node to check degrees: ");
                    scanf("%d", &fromNode);
                    if (fromNode < 0 || fromNode >= graphNodes)
                    {
                        printf("Invalid node index. Try again.\n");
                        break;
                    }
                    printf("In-degree of node %d: %d\n", fromNode, get_in_degree(g, fromNode));
                    printf("Out-degree of node %d: %d\n", fromNode, get_out_degree(g, fromNode));
                    break;

                case 9: // Check if Graph is Connected
                    printf("Is the graph connected? %s\n", is_connected(g) ? "Yes" : "No");
                    break;

                case 0:
                    printf("Returning to Main Menu...\n");
                    break;

                default:
                    printf("Invalid choice. Please try again.\n");
                }
            } while (graph_choice != 0);
            break;

        case 2: // Visualization Algorithms
            initGrid();
            do
            {
                printf("\nChoose algorithm:\n");
                printf("1. Dijkstra's Algorithm\n");
                printf("2. BFS Algorithm\n");
                printf("3. DFS Algorithm\n");
                printf("0. Exit\n");
                printf("Enter your choice (0, 1, 2, or 3): ");
                scanf("%d", &algorithm_choice);

                switch (algorithm_choice)
                {
                case 1:
                    dijkstra_visual();
                    printf("\nShortest path:\n");
                    printPath();
                    break;
                case 2:
                    bfs_visual();
                    printf("\nShortest path:\n");
                    printPath();
                    break;
                case 3:
                    dfs_visual();
                    printf("\nShortest path:\n");
                    printPath();
                    break;
                case 0:
                    printf("Returning to Main Menu...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
                }
            } while (algorithm_choice != 0);
            break;

        case 3: 
        {// friends recommendation system
            int num_users;
            printf("Enter the number of users in the social network: ");
            scanf("%d", &num_users);

            graph *social_network = create_graph(num_users);

            if (!social_network)
            {
                printf("Failed to create the social network graph. Exiting...\n");
                return 1;
            }

            int choice, from, to, user;
            do
            {
                printf("\n===== Friend Recommendation System =====\n");
                printf("1. Add Friendship\n");
                printf("2. Recommend Friends for a User\n");
                printf("0. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1: // Add friendship
                    printf("Enter the user IDs to create a friendship (from to): ");
                    scanf("%d %d", &from, &to);
                    if (from < 0 || from >= num_users || to < 0 || to >= num_users)
                    {
                        printf("Invalid user IDs. Please try again.\n");
                    }
                    else if (add_edge(social_network, from, to))
                    {
                        printf("Friendship added between User %d and User %d.\n", from, to);
                    }
                    else
                    {
                        printf("Failed to add friendship. It might already exist.\n");
                    }
                    break;

                case 2: // Recommend friends
                    printf("Enter the user ID to recommend friends for: ");
                    scanf("%d", &user);
                    recommend_friends(social_network, user);
                    break;

                case 0: // Exit
                    printf("Exiting Friend Recommendation System...\n");
                    break;

                default:
                    printf("Invalid choice. Please try again.\n");
                }
            } while (choice != 0);
            // Clean up
            destroy_graph(social_network);
            break;
        }
        case 0:
            printf("Exiting program...\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (main_choice != 0);

    // Cleanup
    destroy_graph(g);
    return 0;
}

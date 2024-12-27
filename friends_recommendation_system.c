#include <stdio.h>
#include <stdlib.h>
#include "header.h" // Include your graph library header

// Function to recommend friends for a user
void recommend_friends(graph *g, int user) {
    printf("\n===== Friend Recommendations for User %d =====\n", user);

    if (user < 0 || user >= g->numnodes) {
        printf("Invalid user ID. Please try again.\n");
        return;
    }

    int *distances = NULL;
    int *predecessors = NULL;

    // Get shortest paths from the user to all other nodes
    distances = shortest_path_dijkstra(g, user, -1, &predecessors);

    if (!distances) {
        printf("Failed to compute shortest paths.\n");
        return;
    }

    // Recommend friends
    int recommendations = 0; // Count of friend recommendations
    for (int i = 0; i < g->numnodes; i++) {
        if (i == user) continue; // Skip the user themselves

        // Check if not directly connected and reachable
        if (!has_edge(g, user, i) && distances[i] != INF) {
            printf("Recommend User %d\n", i);
            recommendations++;
        }
    }

    if (recommendations == 0) {
        printf("No friend recommendations available for User %d.\n", user);
    }

    free(distances);
    free(predecessors);
}

// Main function for the Friend Recommendation System
int main() {
    int num_users;
    printf("Enter the number of users in the social network: ");
    scanf("%d", &num_users);

    graph *social_network = create_graph(num_users);

    if (!social_network) {
        printf("Failed to create the social network graph. Exiting...\n");
        return 1;
    }

    int choice, from, to, user;
    do {
        printf("\n===== Friend Recommendation System =====\n");
        printf("1. Add Friendship\n");
        printf("2. Recommend Friends for a User\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Add friendship
                printf("Enter the user IDs to create a friendship (from to): ");
                scanf("%d %d", &from, &to);
                if (from < 0 || from >= num_users || to < 0 || to >= num_users) {
                    printf("Invalid user IDs. Please try again.\n");
                } else if (add_edge(social_network, from, to)) {
                    printf("Friendship added between User %d and User %d.\n", from, to);
                } else {
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

    return 0;
}

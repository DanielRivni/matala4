#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph.h"

void create_edge_to_node_from_input(pnode head, int node_number)
{
    int endpoint_number, weight;
    while (scanf("%d %d", &endpoint_number, &weight) > 0)
    {
        printf("creating edge with %d -> %d(%d).\n", node_number, endpoint_number, weight);
        pnode node = get_node(head, node_number);
        pnode endpoint = get_node(head, endpoint_number);

        if (node == NULL || endpoint == NULL)
        {
            break;
        }

        pedge edge = create_edge(endpoint, weight);
        add_edge_to_node(node, edge);
    }
}

int main()
{
    char choice = 0;

    pnode head = NULL;
    int node_number = -1;

    while (1)
    {
        if (scanf("%c", &choice) <= 0)
        {
            return 0;
        }

        switch (choice)
        {
        case 'A':
            int nodes = 0;
            scanf("%d", &nodes);

            printf("creating graph with %d nodes.\n", nodes);
            if (head != NULL)
            {
                delete_graph(head);
            }
            head = create_graph(nodes);
            break;

        case 'S':
            int start = -1, end = -1;
            scanf("%d %d", &start, &end);

            int result = dijkstra(head, start, end);

            printf("Dijsktra shortest path: %d\n", result == INT_MAX ? -1 : result);

            break;

        case 'P':
            printf("print graph.\n");
            print_graph(head);
            break;

        case 'D':
            scanf("%d", &node_number);
            printf("delete node %d and all pointed edges.\n", node_number);
            delete_node(head, node_number);
            break;

        case 'B':
            scanf("%d", &node_number);

            pnode node = get_node(head, node_number);
            if (node == NULL)
            {
                printf("adding node %d.\n", node_number);
                add_node(head, node_number);
            }
            else
            {
                printf("update node %d.\n", node_number);
                clear_node_edges(node);
            }

            create_edge_to_node_from_input(head, node_number);
            break;

        case 'n':
            scanf("%d", &node_number);
            create_edge_to_node_from_input(head, node_number);
            break;

        case '\n':
            // break;
            return 0;
        }
    }
    return 0;
}

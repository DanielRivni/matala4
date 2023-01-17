#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph.h"

#define DEBUG 0
#define debug_print(args...) \
    if (DEBUG)               \
    fprintf(stderr, args)

void create_edge_to_node_from_input(pnode head, int node_number)
{
    int endpoint_number, weight;
    while (scanf("%d %d", &endpoint_number, &weight) > 0)
    {
        debug_print("creating edge with %d -> %d(%d).\n", node_number, endpoint_number, weight);
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
    int result = -1;

    while (1)
    {
        if (scanf("%c", &choice) <= 0)
        {
            break;
        }

        switch (choice)
        {
        case 'A':
            int nodes = 0;
            scanf("%d", &nodes);

            debug_print("creating graph with %d nodes.\n", nodes);
            if (head != NULL)
            {
                delete_graph(head);
            }
            head = create_graph(nodes);
            break;

        case 'S':
            int start = -1, end = -1;
            scanf("%d %d", &start, &end);

            result = dijkstra(head, start, end);

            printf("Dijsktra shortest path: %d \n", result);

            break;

        case 'P':
            debug_print("print graph.\n");
            print_graph(head);
            break;

        case 'D':
            scanf("%d", &node_number);
            debug_print("delete node %d and all pointed edges.\n", node_number);
            delete_node(head, node_number);
            break;

        case 'B':
            scanf("%d", &node_number);

            pnode node = get_node(head, node_number);
            if (node == NULL)
            {
                debug_print("adding node %d.\n", node_number);
                add_node(head, node_number);
            }
            else
            {
                debug_print("update node %d.\n", node_number);
                clear_node_edges(node);
            }

            create_edge_to_node_from_input(head, node_number);
            break;

        case 'n':
            scanf("%d", &node_number);
            create_edge_to_node_from_input(head, node_number);
            break;

        case 'T':
            int number_of_nodes = 0;
            scanf("%d", &number_of_nodes);

            int *tsp_nodes = malloc(number_of_nodes * sizeof(int));

            for (int i = 0; i < number_of_nodes; i++)
            {
                scanf("%d", &node_number);
                tsp_nodes[i] = node_number;
            }

            result = tsp(head, tsp_nodes, number_of_nodes);
            printf("TSP shortest path: %d \n", result);

            free(tsp_nodes);
            break;

        case '\n':
            delete_graph(head);
            return 0;
        }
    }

    delete_graph(head);
    return 0;
}

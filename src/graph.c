#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph.h"

pnode create_graph(int nodes)
{
    if (nodes <= 0)
    {
        return NULL;
    }

    pnode p = NULL;
    for (int i = nodes - 1; i >= 0; i--)
    {
        pnode node = (pnode)malloc(sizeof(node));
        node->node_num = i;
        node->edges = NULL;
        node->next = p;

        p = node;
    }

    return p;
}

void delete_graph(pnode head)
{
    if (head == NULL)
    {
        return;
    }

    pnode p = head;
    while (p != NULL)
    {
        delete_node(p, p->node_num);
        p = p->next;
    }
}

void add_node(pnode head, int node_number)
{
    pnode node = (pnode)malloc(sizeof(node));
    node->node_num = node_number;
    node->edges = NULL;
    node->next = NULL;

    if (head == NULL)
    {
        head = node;
        return;
    }

    pnode p = head;
    pnode previous = NULL;

    while (p != NULL)
    {
        if (p->node_num > node_number)
        {
            if (previous != NULL)
            {
                node->next = p;
                previous->next = node;
            }
            else
            {
                head = node;
            }
            break;
        }

        if (p->next == NULL)
        {
            p->next = node;
            break;
        }

        previous = p;
        p = p->next;
    }
}

pnode get_node(pnode head, int node_number)
{

    pnode p = NULL;

    p = head;
    while (p != NULL && p->node_num != node_number)
    {
        p = p->next;
    }

    return p;
}

void delete_node_edge(pnode node, int endpoint_node_number)
{
    if (node == NULL || node->edges == NULL)
    {
        return;
    }

    pedge p = node->edges;
    pedge previous = NULL;

    while (p != NULL)
    {
        if (p->endpoint->node_num == endpoint_node_number)
        {
            if (previous != NULL)
            {
                previous->next = p->next;
            }
            else
            {
                node->edges = NULL;
            }

            free(p);
            return;
        }

        previous = p;
        p = p->next;
    }
}

void delete_node(pnode head, int node_number)
{
    if (head == NULL)
    {
        return;
    }

    pnode p = NULL;
    p = head;
    while (p != NULL)
    {
        delete_node_edge(p, node_number);
        p = p->next;
    }

    p = head;
    pnode previous = NULL;

    while (p != NULL)
    {
        if (p->node_num == node_number)
        {
            if (previous != NULL)
            {
                previous->next = p->next;
            }
            else
            {
                head = p->next;
            }

            free(p);
            return;
        }

        previous = p;
        p = p->next;
    }
}

pedge create_edge(pnode endpoint, int weight)
{

    if (endpoint == NULL)
    {
        return NULL;
    }

    pedge edge = (pedge)malloc(sizeof(edge));
    edge->endpoint = endpoint;
    edge->weight = weight;
    edge->next = NULL;

    return edge;
}

void add_edge_to_node(pnode node, pedge edge)
{
    if (node == NULL)
    {
        return;
    }

    if (node->edges == NULL)
    {
        node->edges = edge;
        return;
    }

    pedge p = node->edges;
    while (p->next != NULL)
    {
        p = p->next;
    }

    p->next = edge;
}

void clear_node_edges(pnode node)
{
    if (node == NULL)
    {
        return;
    }

    pedge p = node->edges;
    while (p != NULL)
    {
        pedge tmp = p;
        p = p->next;

        free(tmp);
    }

    node->edges = NULL;
}

void print_edges(int node_number, pedge edges)
{
    pedge p = edges;
    while (p != NULL)
    {
        printf("%d -> %d(%d) ", node_number, p->endpoint->node_num, p->weight);
        p = p->next;
    }

    printf("\n");
}

void print_graph(pnode head)
{

    pnode p = head;
    while (p != NULL)
    {
        printf("%d ", p->node_num);
        print_edges(p->node_num, p->edges);
        p = p->next;
    }

    printf("\n");
}

int get_graph_nodes(pnode head)
{
    pnode p = head;
    int i = 0;
    while (p != NULL)
    {
        p = p->next;
        i++;
    }
    return i;
}

int dijkstra(pnode head, int start, int end)
{

    int num_nodes = get_graph_nodes(head);
    if (start < 0 || end > num_nodes - 1)
    {
        return INT_MAX;
    }

    int *distances = malloc(num_nodes * sizeof(int));
    int *visited = malloc(num_nodes * sizeof(int));
    int i, j, min_distance, next_node;
    pnode current_node = NULL;
    pedge current_edge = NULL;

    for (i = 0; i < num_nodes; i++)
    {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }

    distances[start] = 0;

    for (i = 0; i < num_nodes; i++)
    {
        min_distance = INT_MAX;
        for (j = 0; j < num_nodes; j++)
        {
            if (!visited[j] && distances[j] < min_distance)
            {
                min_distance = distances[j];
                next_node = j;
            }
        }

        visited[next_node] = 1;
        current_node = get_node(head, next_node);
        if (current_node != NULL)
        {
            current_edge = current_node->edges;
            while (current_edge != NULL)
            {
                if (distances[next_node] + current_edge->weight < distances[current_edge->endpoint->node_num])
                {
                    distances[current_edge->endpoint->node_num] = distances[next_node] + current_edge->weight;
                }
                current_edge = current_edge->next;
            }
        }
    }

    int result = distances[end];
    free(distances);
    free(visited);
    return result;
}

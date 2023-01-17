#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "graph.h"

#define DEBUG 0
#define debug_print(args...) \
    if (DEBUG)               \
    fprintf(stderr, args)

pnode create_graph(int nodes)
{
    if (nodes <= 0)
    {
        return NULL;
    }

    pnode p = NULL;
    for (int i = nodes - 1; i >= 0; i--)
    {
        pnode _node = (pnode)malloc(sizeof(node));
        _node->node_num = i;
        _node->edges = NULL;
        _node->next = p;

        p = _node;
    }

    return p;
}

void free_edges(pedge edges)
{
    pedge current = edges;
    while (current != NULL)
    {
        pedge next = current->next;
        free(current);
        current = next;
    }
}

void delete_graph(pnode head)
{
    pnode current = head;
    while (current != NULL)
    {
        pnode next = current->next;
        free_edges(current->edges);
        free(current);
        current = next;
    }
}

void add_node(pnode head, int node_number)
{
    pnode _node = (pnode)malloc(sizeof(node));
    _node->node_num = node_number;
    _node->edges = NULL;
    _node->next = NULL;

    if (head == NULL)
    {
        head = _node;
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
                _node->next = p;
                previous->next = _node;
            }
            else
            {
                head = _node;
            }
            break;
        }

        if (p->next == NULL)
        {
            p->next = _node;
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

pedge get_edge(pnode node, int node_number)
{

    pedge p = NULL;

    p = node->edges;
    while (p != NULL && p->endpoint != NULL && p->endpoint->node_num != node_number)
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
                node->edges = p->next;
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

    pnode p = head;
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

            free_edges(p->edges);
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

    pedge _edge = (pedge)malloc(sizeof(edge));
    _edge->endpoint = endpoint;
    _edge->weight = weight;
    _edge->next = NULL;

    return _edge;
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

pnode get_node_by_index(pnode head, int index)
{
    int i = 0;
    pnode p = head;
    while (p != NULL)
    {
        if (i == index)
        {
            return p;
        }
        p = p->next;
        i++;
    }
    return NULL;
}

int get_node_index(pnode head, int node_num)
{
    int i = 0;
    pnode p = head;
    while (p != NULL)
    {
        if (p->node_num == node_num)
        {
            return i;
        }
        i++;
        p = p->next;
    }
    return -1;
}

int dijkstra(pnode head, int start, int end)
{
    int num_nodes = get_graph_nodes(head);
    int start_index = get_node_index(head, start);
    int end_index = get_node_index(head, end);

    if (start_index == -1 || end_index == -1)
    {
        return -1;
    }

    int *distances = (int *)malloc(num_nodes * sizeof(int));
    int *visited = (int *)malloc(num_nodes * sizeof(int));
    int min_distance = INT_MAX;
    pnode current_node = NULL;
    pedge current_edge = NULL;

    // initialize distances
    for (int i = 0; i < num_nodes; i++)
    {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }

    // distance to start itself is 0
    distances[start_index] = 0;

    for (int i = 0; i < num_nodes; i++)
    {
        min_distance = INT_MAX;
        int next_node = -1;

        // find min distance for next search
        for (int j = 0; j < num_nodes; j++)
        {
            if (!visited[j] && distances[j] < min_distance)
            {
                min_distance = distances[j];
                next_node = j;
            }
        }

        if (next_node != -1)
        {
            visited[next_node] = 1;
            current_node = get_node_by_index(head, next_node);
            if (current_node != NULL)
            {
                current_edge = current_node->edges;
                while (current_edge != NULL)
                {
                    // if found a lower distance, override current one
                    int index = get_node_index(head, current_edge->endpoint->node_num);
                    if (index != -1 && distances[next_node] != INT_MAX && distances[next_node] + current_edge->weight < distances[index])
                    {
                        distances[index] = distances[next_node] + current_edge->weight;
                    }
                    current_edge = current_edge->next;
                }
            }
        }
    }

    // return distance to end
    int result = distances[end_index];

    free(distances);
    free(visited);
    return result == INT_MAX ? -1 : result;
}

void permute(int *list, int start, int end, int **result, int *count)
{
    if (start == end)
    {
        for (int i = 0; i <= end; i++)
        {
            result[*count][i] = list[i];
        }
        (*count)++;
    }
    else
    {
        for (int i = start; i <= end; i++)
        {
            int temp = list[start];
            list[start] = list[i];
            list[i] = temp;
            permute(list, start + 1, end, result, count);
            temp = list[start];
            list[start] = list[i];
            list[i] = temp;
        }
    }
}

int factorial(int n)
{
    int num_permutations = 1;
    for (int i = 1; i <= n; i++)
    {
        num_permutations *= i;
    }

    return num_permutations;
}

void print_permutation(int *permute, int size, int cost)
{
    for (int i = 0; i < size; i++)
    {
        debug_print("%d ", permute[i]);
    }
    debug_print(" - %d\n", cost);
}

int tsp(pnode head, int *nodes, int number_of_nodes)
{
    // create all permutations
    int number_of_permutation = factorial(number_of_nodes);
    int **paths = (int **)malloc(sizeof(int *) * number_of_permutation);
    for (int i = 0; i < number_of_permutation; i++)
    {
        paths[i] = (int *)malloc(sizeof(int) * number_of_nodes);
    }

    int count = 0;
    permute(nodes, 0, number_of_nodes - 1, paths, &count);

    int *results = (int *)malloc(sizeof(int) * number_of_permutation);

    // for each permutation, sum path weight
    for (int i = 0; i < number_of_permutation; i++)
    {
        results[i] = 0;

        for (int j = 0; j < number_of_nodes - 1; j++)
        {
            int d = dijkstra(head, paths[i][j], paths[i][j + 1]);
            if (d != -1)
            {
                results[i] += d;
            }
            else
            {
                // mark this permutation as failed, and break
                results[i] = -1;
                break;
            }
        }

        print_permutation(paths[i], number_of_nodes, results[i]);
    }

    int min = INT_MAX;
    for (int i = 0; i < number_of_permutation; i++)
    {
        if (results[i] != -1 && results[i] < min)
        {
            min = results[i];
        }
    }

    // free all resources
    for (int i = 0; i < number_of_permutation; i++)
    {
        free(paths[i]);
    }
    free(paths);
    free(results);

    return min == INT_MAX ? -1 : min;
}
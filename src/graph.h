#ifndef GRAPH_
#define GRAPH_

typedef struct GRAPH_NODE_ *pnode;

typedef struct edge_
{
    int weight;
    pnode endpoint;
    struct edge_ *next;
} edge, *pedge;

typedef struct GRAPH_NODE_
{
    int node_num;
    pedge edges;
    struct GRAPH_NODE_ *next;
} node, *pnode;

pnode create_graph(int nodes);
void delete_graph(pnode head);
void print_graph(pnode head);

void add_node(pnode head, int node_number);
pnode get_node(pnode head, int node_number);
void delete_node(pnode head, int node_number);

pedge create_edge(pnode endpoint, int weight);
void add_edge_to_node(pnode node, pedge edge);
void clear_node_edges(pnode node);

int dijkstra(pnode head, int start, int end);
#endif

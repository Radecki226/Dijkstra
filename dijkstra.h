#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

//Structure for node reprezentation in graph adjacency list
typedef struct node_t {
  int vertex;
  struct node_t* next;
} node_t;

//Structure for graph reprezentation, elements of adjacent_list are linked lists
typedef struct graph_t {
  int n_vertices;
  node_t** adjacent_list;
} graph_t;

//Node for dijkstra algorithm, one additional field- cost is needed
typedef struct dijkstra_node_t {
  int vertex;
  int* cost;
  struct dijkstra_node_t* next;
} dijkstra_node_t;

//Type returned by dijkstra algorithm
typedef struct dijkstra_return_type_t {
  int* cost_array;
  int* previous_vertices_array;
} dijkstra_return_type_t;

//create vertices
void graph_init(graph_t* graph, int n_vertices);

void graph_add_edge(graph_t* graph, int vertex_a, int vertex_b);

void graph_cleanup(graph_t* graph);

void graph_print(graph_t* graph);

int dijkstra_find_min_vertex(dijkstra_node_t* list);

void dijkstra_pop_min_vertex(dijkstra_node_t** list, int vertex);

void dijkstra_return_type_init(dijkstra_return_type_t* return_data, int n_vertices);

void dijkstra_return_type_deInit(dijkstra_return_type_t* return_data);

void dijkstra(dijkstra_return_type_t* return_data, graph_t* graph, int source_vertex);

#endif
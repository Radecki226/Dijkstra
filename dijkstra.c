#include "dijkstra.h"

//create vertices
void graph_init(graph_t* graph, int n_vertices) {
  graph->n_vertices = n_vertices;
  graph->adjacent_list = malloc(n_vertices * sizeof(node_t*));

  // there are no edges in the beginning
  for (int n = 0; n < n_vertices; n++){
    graph->adjacent_list[n] = NULL;
  }
}

void graph_add_edge(graph_t* graph, int vertex_a, int vertex_b) {
  // vertex_a -> vertex_b
  node_t* node_a_b = malloc(sizeof(node_t));
  // Push move beginning of the list to new node and point new node to previous beginning
  node_a_b->vertex = vertex_b;
  node_a_b->next = graph->adjacent_list[vertex_a];
  graph->adjacent_list[vertex_a] = node_a_b;

  // vertex_b -> vertex_a
  node_t* node_b_a = malloc(sizeof(node_t));
  node_b_a->vertex = vertex_a;
  node_b_a->next = graph->adjacent_list[vertex_b];
  graph->adjacent_list[vertex_b] = node_b_a;
}

void graph_cleanup(graph_t* graph) {
  for (int i = 0; i < graph->n_vertices; i++) {
    node_t* iterator = graph->adjacent_list[i];
    while(iterator != NULL) {
      node_t* to_be_freed = iterator;
      iterator = iterator->next;
      free(to_be_freed);
    }
  }
  free(graph->adjacent_list);  
}

void graph_print(graph_t* graph) {
  for (int i = 0; i < graph->n_vertices; i++) {
    printf("%d : [", i);
    node_t* iterator = graph->adjacent_list[i];
    while(iterator != NULL) {
      printf("%d, ", iterator->vertex);
      iterator = iterator->next;
    }
    printf("]\n");
  }
}

//Find vertex with the smallest cost
int dijkstra_find_min_vertex(dijkstra_node_t* list) {
  dijkstra_node_t* iter = list;
  if (list == NULL) {
    return -1;
  }

  int min = *(iter->cost);
  int min_vertex = iter->vertex;
  
  while(iter != NULL) {
    int cost = *(iter->cost);
    int vertex = iter->vertex;
    if (cost < min) {
      min = cost;
      min_vertex = vertex;
    }
    iter = iter->next;
  }

  return min_vertex;
}

//Delete min vertex from the list after fetching it
void dijkstra_pop_min_vertex(dijkstra_node_t** list, int vertex) {
  dijkstra_node_t* iter = *list;
  
  //delete from beginning
  if (iter->vertex == vertex){
    //printf("list old head vertex = %d\n", &list->vertex);
    dijkstra_node_t* new_head = iter->next;
    *list = iter->next;
    //printf("list new head vertex = %d\n", *list->vertex);
    free(iter);
    return;
  }

  //detele other element
  while(iter->next != NULL) {
    if (vertex == iter->next->vertex){
      printf("vertex to pop = %d\n", iter->next->vertex);
      dijkstra_node_t* vertex_to_pop = iter->next;
      iter->next = iter->next->next;
      vertex_to_pop->next = NULL;
      free(vertex_to_pop);
      break;
    }
    iter = iter->next;
  }

}

void dijkstra_return_type_init(dijkstra_return_type_t* return_data, int n_vertices){
  return_data->cost_array = malloc(n_vertices * sizeof(int));
  return_data->previous_vertices_array = malloc(n_vertices * sizeof(int));
}

void dijkstra_return_type_deInit(dijkstra_return_type_t* return_data){
  free(return_data->cost_array);
  free(return_data->previous_vertices_array);
}

void dijkstra(dijkstra_return_type_t* return_data, graph_t* graph, int source_vertex) {
  //Init
  //In simple words- array containing individual vertices costs
  int* dijkstra_cost_array = return_data->cost_array;

  //Array which constains index of previous vertex in shortest path for each vertex
  int* dijkstra_previous_vertices_array = return_data->previous_vertices_array;

  //List of all non-visited nodes
  dijkstra_node_t* dijkstra_non_visited_nodes = malloc(sizeof(dijkstra_node_t));

  //Initialize cost array with NON_VISITED values apart from source vertex which should be initialized with 0
  for (int i = 0; i < graph->n_vertices; i++) {
    if (i == source_vertex) {
      dijkstra_cost_array[i] = 0;
      dijkstra_previous_vertices_array[i] = i;
    } else {
      dijkstra_cost_array[i] = INT_MAX;
    }
  }
    
  //Initially all vertices must be added to the list of non_visited veritces
  dijkstra_non_visited_nodes = NULL;
  for (int i = 0; i < graph->n_vertices; i++) {
    dijkstra_node_t* new_node = malloc(sizeof(dijkstra_node_t));
    new_node->vertex = i;
    new_node->cost = &dijkstra_cost_array[i];
    int temp = *(new_node->cost);
    new_node->next = dijkstra_non_visited_nodes;
    dijkstra_non_visited_nodes = new_node;
  }

  //Init phase ended, now start algorithm do algorithm until there are no vertices in non visited nodes list
  while(dijkstra_non_visited_nodes != NULL) {

    //Fetch minimum vertex
    int min_vertex = dijkstra_find_min_vertex(dijkstra_non_visited_nodes);
    int min_vertex_cost = dijkstra_cost_array[min_vertex];

    //Pop it from list
    dijkstra_pop_min_vertex(&dijkstra_non_visited_nodes, min_vertex);

    //Do for all edges incident with min_vertex
    node_t* incident_vertices_iter;
    incident_vertices_iter = graph->adjacent_list[min_vertex];
    while(incident_vertices_iter != NULL){
      //u is vertex incident to our min_vertex, if path through min_vertex(potential cost) is shorter than current shortest update cost and previous vertex
      int u = incident_vertices_iter->vertex;
      int current_cost = dijkstra_cost_array[u];
      int potential_cost = dijkstra_cost_array[min_vertex] + 1;
      if (potential_cost < current_cost) {
        dijkstra_cost_array[u] = potential_cost;
        dijkstra_previous_vertices_array[u] = min_vertex;
      }
      incident_vertices_iter = incident_vertices_iter->next;
    }
  } 
  free(dijkstra_non_visited_nodes);
  return_data->cost_array = dijkstra_cost_array;
  return_data->previous_vertices_array = dijkstra_previous_vertices_array;
}
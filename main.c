#include "dijkstra.h"

int main(){
  graph_t graph;
  int graph_size = 10;
  graph_init(&graph, graph_size);
  graph_add_edge(&graph, 0, 1);
  graph_add_edge(&graph, 0, 4);
  graph_add_edge(&graph, 0, 5);
  graph_add_edge(&graph, 1, 2);
  graph_add_edge(&graph, 2, 3);
  graph_add_edge(&graph, 2, 6);
  graph_add_edge(&graph, 3, 4);
  graph_add_edge(&graph, 4, 5);
  graph_add_edge(&graph, 4, 8);
  graph_add_edge(&graph, 6, 7);
  graph_add_edge(&graph, 7, 8);
  graph_add_edge(&graph, 8, 9);
  graph_print(&graph);

  dijkstra_return_type_t return_data;
  dijkstra_return_type_init(&return_data, graph_size);

  dijkstra(&return_data, &graph, 5);

  for (int i = 0; i < graph_size; i++){
      printf("Cost of %d = %d, previous vertix for %d = %d\n", i, return_data.cost_array[i], i, return_data.previous_vertices_array[i]);
  }


  dijkstra_return_type_deInit(&return_data);
  graph_cleanup(&graph);

  printf("Goodbye!\n");
  return 0;
}
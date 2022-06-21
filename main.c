#include "dijkstra.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LEN 100
#define GRAPH_SIZE   15

int main(){
  int dest_vertix;
  bool start_vertices[GRAPH_SIZE];
  memset(start_vertices, false, sizeof(start_vertices));
  graph_t graph;
  graph_init(&graph, GRAPH_SIZE);

  FILE *fp;
  char line[MAX_LINE_LEN];

  fp = fopen("input.txt", "r");
  if (fp == NULL) {
    printf("Problem while reading file\n");
    graph_cleanup(&graph);
    return -1;
  }
  char *pch;
  size_t line_counter = 0;
  while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
    pch = strtok(line, ": ,\n");
    if (line_counter == 0) {
      if (strcmp(pch, "D")) {
        printf("Error: first line should be: 'D: {index of destination vertix}'.\n");
        graph_cleanup(&graph);
        return -1;
      }
      pch = strtok(NULL, ": ,\n");
      if (pch == NULL) {
        printf("Error: first line should be: 'D: {index of destination vertix}'.\n");
        graph_cleanup(&graph);
        return -1;
      }
      dest_vertix = atoi(pch);
    } else if (line_counter == 1) {
      if (strcmp(pch, "S")) {
        printf("Error: second line should be: 'S: {index of first start vertix},{index of second start vertix} etc.'.\n");
        graph_cleanup(&graph);
        return -1;
      }
      pch = strtok(NULL, ": ,\n");
      if (pch == NULL) {
        printf("Error: second line should be: 'S: {index of first start vertix},{index of second start vertix} etc.'.\n");
        graph_cleanup(&graph);
        return -1;
      }
      do {
        start_vertices[atoi(pch)] = true;
        pch = strtok(NULL, ": ,\n");
      } while (pch != NULL);
    } else {
      if (pch == NULL) {
        line_counter++;
        continue;
      }

      int first_vertix = atoi(pch);
      pch = strtok(NULL, ": ,\n");
      if (pch == NULL) {
        line_counter++;
        continue;
      }
      graph_add_edge(&graph, first_vertix, atoi(pch));
    }
    line_counter++;
  }
  if (line_counter < 3) {
    printf("File have too few lines.\n");
    graph_cleanup(&graph);
    return -1;
  }

  printf("Loaded graph:\n");
  graph_print(&graph);
  printf("Potential starting points: ");
  for (int i = 0; i < GRAPH_SIZE; i++) {
    if (start_vertices[i]) {
      printf("%d, ", i);
    }
  }
  printf("\nDestination point: %d\n", dest_vertix);

  dijkstra_return_type_t return_data;
  dijkstra_return_type_init(&return_data, GRAPH_SIZE);

  dijkstra(&return_data, &graph, dest_vertix);

  int min_cost = START_COST;
  int best_starting_vertix = -1;
  for (size_t i = 0; i < GRAPH_SIZE; i++) {
    if (start_vertices[i]) {
      if (return_data.cost_array[i] < min_cost) {
        best_starting_vertix = i;
        min_cost = return_data.cost_array[i];
      }
    }
  }
  if (best_starting_vertix == -1) {
    printf("Route not found\n");
  } else {
    printf("Route cost: %d\n", min_cost);
    printf("Found Route: %d -> %d", best_starting_vertix, return_data.previous_vertices_array[best_starting_vertix]);
    int cur_vertix = return_data.previous_vertices_array[best_starting_vertix];
    while (cur_vertix != dest_vertix) {
      printf(" -> %d", return_data.previous_vertices_array[cur_vertix]);
      cur_vertix = return_data.previous_vertices_array[cur_vertix];
    }
    printf("\n");
  }

  dijkstra_return_type_deInit(&return_data);
  graph_cleanup(&graph);

  printf("Goodbye!\n");
  return 0;
}
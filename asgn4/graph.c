#include "graph.h"

#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}

uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if ((i >= 0 && i < VERTICES) && (j >= 0 && j < VERTICES)) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if (G->matrix[i][j] > 0) {
        return true;
    }
    return false;
}

uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (graph_has_edge(G, i, j)) {
        return G->matrix[i][j];
    }
    return 0;
}

bool graph_visited(Graph *G, uint32_t v) {
    return G->visited[v];
}

void graph_mark_visited(Graph *G, uint32_t v) {
    if (v > 0 && v < VERTICES) {
        G->visited[v] = true;
    }
}

void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v > 0 && v < VERTICES) {
        G->visited[v] = false;
    }
}

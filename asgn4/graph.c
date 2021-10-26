#include "graph.h"

#include "vertices.h"

#include <inttypes.h>
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
    if ((i >=0 && i <= G->vertices) && (j >= 0 && j <= G->vertices) && (!(i == 0 && j == 0))) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if ((i >= 0 && i < G->vertices) && (j >= 0 && j < G->vertices)) {
        if (G->matrix[i][j] > 0) {
            return true;
        }
    }
    return false;
}

uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if ((i >= 0 && i < G->vertices) && (j >= 0 && j < G->vertices)) {
        if (G->matrix[i][j] > 0) {
            return G->matrix[i][j];
        }
    }
    return 0;
}

bool graph_visited(Graph *G, uint32_t v) {
    return G->visited[v];
}

void graph_mark_visited(Graph *G, uint32_t v) {
    if (v >= 0 && v <= G->vertices) {
        G->visited[v] = true;
    }
}

void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v >= 0 && v <= G->vertices) {
        G->visited[v] = false;
    }
}

void graph_print(Graph *G) {
    for (uint32_t i = 0; i < G->vertices; i += 1) {
        for (uint32_t j = 0; j < G->vertices; j += 1) {
            printf("%13" PRIu32, G->matrix[i][j]);
        }
    }
}


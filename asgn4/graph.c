#include "graph.h"

#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//Variables that contains the definitions or structure needed for a Graph to work (makes an adjacency matrix)
struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

//Dynamically allocates memory and creates a graph array
//Returns the Graph array when made
//
//vertices: contains the amount of vertices that make up the graph
//undirected: tells wether the graph so be an undirected or directed graph
Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

//Frees the memory of the graph and 0s the memory out to delete it
//
//G: the adjacency matrix graph
void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}

//Returns the amount of vertices in the graph
//
//G: the adjacency matrix graph
uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

//Adds an edge to the graph given the vertices coordinates and the weight of the edge if within bounds
//Returns true if the edge was successfully added
//Returns false if the edge was not able to be added
//
//G: the adjacency matrix graph
//i: vertex i
//j: vertex j
//k: the weight of the edge made of i and j
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if ((i >= 0 && i <= G->vertices) && (j >= 0 && j <= G->vertices) && (!(i == 0 && j == 0))) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

//Shows whether there is an edge at the given vertex if within bounds
//Returns true if there is an edge
//Returns false if there is not an edge
//
//G: the adjacency matrix graph
//i: vertex i
//j: vertex j
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if ((i >= 0 && i < G->vertices) && (j >= 0 && j < G->vertices)) {
        if (G->matrix[i][j] > 0) {
            return true;
        }
    }
    return false;
}

//Gives the weight of the edge if there exists an edge there if within bounds
//Returns the edge weight if there is an edge weight greater than zero
//Returns zero if there is not an edge or if the weight is zero
//
//G: the adjacency matrix graph
//i: vertex i
//j: vertex j
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if ((i >= 0 && i < G->vertices) && (j >= 0 && j < G->vertices)) {
        if (G->matrix[i][j] > 0) {
            return G->matrix[i][j];
        }
    }
    return 0;
}

//Shows whether the vertex provided has been visited or not
//Returns true if vertex is visited
//Returns false if vertex is not visited
//
//G: the adjacency matrix graph
//v: the vertex
bool graph_visited(Graph *G, uint32_t v) {
    return G->visited[v];
}

//If the vertex is within bounds then the vertex will be set to visited
//
//G: the adjacency matrix graph
//v: the vertex
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v >= 0 && v <= G->vertices) {
        G->visited[v] = true;
    }
}

//If the vertex is within bounds then the vertex will be set to unvisited
//
//G: the adjacency matrix graph
//v: the vertex
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v >= 0 && v <= G->vertices) {
        G->visited[v] = false;
    }
}

//Debug print function to check if graph is proper
//
//G: the adjacency matrix graph
void graph_print(Graph *G) {
    for (uint32_t i = 0; i < G->vertices; i += 1) {
        for (uint32_t j = 0; j < G->vertices; j += 1) {
            printf("%13" PRIu32, G->matrix[i][j]);
        }
    }
}

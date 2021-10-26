#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//Makes variables that contains the definitions or structure of the Path
struct Path {
    Stack *vertices;
    uint32_t length;
};

//Dynamically allocates memory and creates the path of vertices using stack
//Returns the path array
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    if (p) {
        p->length = 0;
        p->vertices = stack_create(VERTICES + 1);
    }
    return p;
}

//Frees the path stack array and its vertices then zeros its memory
//
//p: the path array
void path_delete(Path **p) {
    if (*p) {
        stack_delete(&(*p)->vertices);
        free(*p);
        *p = NULL;
    }
    return;
}

//Pushes or adds the vertex onto the path array and adds to path length based on vertex edge weight
//Returns true if vertex was pushed
//Returns false if vertex could not be pushed
//
//p: the path array
//v: the vertex
//G: the adjacency matrix graph
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    uint32_t temp = 0;
    stack_peek(p->vertices, &temp);
    if (stack_push(p->vertices, v)) {
        p->length += graph_edge_weight(G, temp, v);
        return true;
    }
    return false;
}

//Pops or removes the vertex from the path array and decrements to path length based on vertex edge weight
//Returns true if vertex was popped
//Returns false if vertex could not be popped
//
//p: the path array
//v: the place to store the popped vertex at
//G: the adjacency matrix graph
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (stack_pop(p->vertices, v)) {
        uint32_t temp = 0;
        stack_peek(p->vertices, &temp);
        p->length -= graph_edge_weight(G, temp, *v);
        return true;
    }
    return false;
}

//Gives the amount of vertices on the path
//Returns the amount of vertices of the path
//
//p: the path array
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

//Gives the length of the path
//Returns the length of the path
//
//p: the path array
uint32_t path_length(Path *p) {
    return p->length;
}

//Copies the vertices of the path from the source to a destination array
//
//dst: the array the vertices are copied to
//src: the source array of the vertices
void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
}

//Prints out the path length and path of cities to an outfile
//
//p: the path array
//outfile: the file to print the output to
//cities: the cities in the path
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %d\nPath: ", p->length);
    stack_print(p->vertices, outfile, cities);
}

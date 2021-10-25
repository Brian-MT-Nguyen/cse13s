#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Path {
    Stack *vertices;
    uint32_t length;
};

Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    if (p) {
        p->length = 0;
        p->vertices = stack_create(VERTICES + 1);
    }
    return p;
}

void path_delete(Path **p) {
    if (*p) {
        stack_delete(&(*p)->vertices);
        free(*p);
        *p = NULL;
    }
    return;
}

bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    uint32_t temp = 0;
    stack_peek(p->vertices, &temp);
    if (stack_push(p->vertices, v)) {
        p->length += graph_edge_weight(G, temp, v);
        return true;
    }
    return false;
}

bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (stack_pop(p->vertices, v)) {
        uint32_t temp = 0;
        stack_peek(p->vertices, &temp);
        p->length -= graph_edge_weight(G, temp, *v);
        return true;
    }
    return false;
}

uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_length(Path *p) {
    return p->length;
}

void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
}

void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %d\n Path:", p->length);
    stack_print(p->vertices, outfile, cities);
}


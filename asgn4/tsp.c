#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "hvui:o:"

void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile);

int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *outfile = stdout;

    int opt = 0;
    bool run_verbose = false;
    bool undirected = false;
    bool help = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': run_verbose = true; break;
        case 'u': undirected = true; break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'h': help = true; break;
        }
    }

    if (help) {
        printf("SYNOPSIS\n");
        printf("  Traveling Salesman Problem using DFS.\n");

        printf("USAGE\n");
        printf("  ./tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n");

        printf("OPTIONS\n");
        printf("  -u             Use undirected graph.\n");
        printf("  -v             Enable verbose printing.\n");
        printf("  -h             Program usage and help.\n");
        printf("  -i infile      Input containing graph (default: stdin)\n");
        printf("  -o outfile     Output of computed path (default: stdout)\n");
    }

    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t weight = 0;

    uint32_t total_vertices = 0;
    scanf("%d", &total_vertices);

    char *cities[1024];
    char input_graph[1024];
    for (uint32_t index = 0; index <= total_vertices; index++) {
        fgets(input_graph, 1024, infile);
        input_graph[strlen(input_graph) - 1] = '\0';
        cities[index] = strdup(input_graph);
    }

    Graph *G = graph_create(total_vertices, undirected);
    if (G == NULL) {
        fprintf(stderr, "Error creating graph.");
        graph_delete(&G);
    }

    while (NULL != fgets(input_graph, 1024, infile)) {
        sscanf(input_graph, "%u %u %u", &i, &j, &weight);
        graph_has_edge(G, i, j);
    }

    //Path *current;
    //Path *shortest;
}

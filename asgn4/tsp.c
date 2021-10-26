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

static uint32_t r_counter = 0;
void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile,
    bool *run_verbose) {
    r_counter++;
    graph_mark_visited(G, v);
    path_push_vertex(curr, v, G);
    uint32_t vertex_store = 0;
    for (uint32_t i = 0; i < graph_vertices(G); i++) {
        if (graph_has_edge(G, v, i) == true) {
            if (graph_visited(G, i) == false) {
                dfs(G, i, curr, shortest, cities, outfile, run_verbose);
            } else if ((i == START_VERTEX) && (path_vertices(curr) == graph_vertices(G))) {
                path_push_vertex(curr, i, G);
                if (path_length(shortest) == 0 || (path_length(shortest) > path_length(curr))) {
                    path_copy(shortest, curr);
                    if (run_verbose) {
                        path_print(curr, outfile, cities);
                    }
                }
            }
        }
    }
    path_pop_vertex(curr, &vertex_store, G);
    graph_mark_unvisited(G, v);
}

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

    //Initializes variables to store input from input file (infile)
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t weight = 0;
    uint32_t total_vertices = 0;

    //Grabs total number of vertices from input file
    int input_vertices = fscanf(infile, "%d\n", &total_vertices);
    //Error Msg for Invalid Vertices
    if (total_vertices > VERTICES || input_vertices != 1) {
        fprintf(stderr, "Error: Invalid number of vertices");
        return -1;
    }

    //Make array to store cities from input file
    char **cities = (char **) calloc(total_vertices, sizeof(char *));

    //Intialize variable that reads from input file
    char input[1024];

    //scans through the first vertices number of input file and stores to cities array
    for (uint32_t i = 0; i < total_vertices; i++) {
        fgets(input, 1024, infile);
        //removes newline character from fgets
        input[strlen(input) - 1] = '\0';
        cities[i] = strdup(input);
    }

    Graph *G = graph_create(total_vertices, undirected);
    //Initialize variable to get the edge input to add to the graph
    int input_triples;

    //scans through all lines that contain the three integer format and adds edges to graph
    while ((input_triples = fscanf(infile, "%d %d %d", &i, &j, &weight)) != EOF) {
        if (input_triples == 3) {
            graph_add_edge(G, i, j, weight);
        }
        //Prints error if input does not fit format
        else {
            fprintf(stderr, "Error: Edge input is malformed.");
            free(cities);
            graph_delete(&G);
            return -1;
        }
    }

    //Creating Paths
    Path *current = path_create();
    Path *shortest = path_create();

    //Calling dfs to search for path
    dfs(G, START_VERTEX, current, shortest, cities, outfile, &run_verbose);

    //print out path
    if (path_length(shortest) > 0) {
        path_print(shortest, outfile, cities);
        fprintf(outfile, "Total recursive calls: %d\n", r_counter);
    }

    //free memory
    graph_delete(&G);
    path_delete(&current);
    path_delete(&shortest);
    free(cities);
    fclose(infile);
    fclose(outfile);
}

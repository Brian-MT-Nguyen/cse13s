#include "node.h"
#include "stack.h"
#include "code.h"
#include "pq.h"
#include "huffman.h"
#include "io.h"
#include "defines.h"
#include "header.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>

#define OPTIONS "hi:o:v"

int main(int argc, char **argv) {
    //Initialize variables used to parse through command line
    int opt = 0;
    bool compression_stats = false;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    bool help = false;

    //Parses through command line input options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': compression_stats = true; break;
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC); break;
        case 'h': help = true; break;
        }
    }

    if (outfile != STDOUT_FILENO) {
        close(outfile);
    }

    //Prints help message if prompted
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

    //Initialize variables to make histogram
    uint64_t hist[ALPHABET] = { 0 };
    uint32_t special_symbols = 0;
    uint8_t buffer[BLOCK] = { 0 };
    hist[0]++;
    hist[255]++;

    //Reads through infile and makes appends to histogram
    uint8_t bytes_read;
    while ((bytes_read = read_bytes(infile, buffer, BLOCK)) > 0) {
        for (uint8_t i = 0; i < bytes_read; i++) {
            if (hist[buffer[i]] == 0) {
                special_symbols += 1;
            }
            hist[buffer[i]] += 1;
        }
    }

    //Make Code table
    Node *root = build_tree(hist);
    Code table[ALPHABET] = { 0 };
    build_codes(root, table);

    //Get Headers and Permissions
    struct stat permissions;
    fstat(infile, &permissions);
    fchmod(outfile, permissions.st_mode);

    Header header = { 0, 0, 0, 0 };
    header.magic = MAGIC;
    header.permissions = permissions.st_mode;
    header.tree_size = (3 * special_symbols - 1);
    header.file_size = permissions.st_size;

    //Write Header to Outfile
    write_bytes(outfile, (uint8_t *) &header, sizeof(header));

    //Write Tree to Outfile
    dump_tree(outfile, root);

    //Write to outfile from infile
    lseek(infile, 0, SEEK_SET);

    while ((bytes_read = read_bytes(infile, buffer, BLOCK)) > 0) {
        //using same var from reading for histogram
        for (uint8_t i = 0; i < bytes_read; i++) {
            write_code(outfile, &table[buffer[i]]);
        }
    }

    //flushing remaining bits
    flush_codes(outfile);

    //close and free
    node_delete(&root);
    delete_tree(&root);
    close(outfile);
    close(infile);
}

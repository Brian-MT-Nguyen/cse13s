#include "huffman.h"
#include "pq.h"

#include <stdio.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
	PriorityQueue *q;
	for(uint32_t i = 0; i < ALPHABET; i++) {
		if(hist->frequency[i] > 0) {
		enqueue(q, hist->symbol);
		}
	}
}

void build_codes(Node *root, Code table[static ALPHABET]) {

}

void dump_tree(int outfile, Node *root) {

}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {

}

void delete_tree(Node **root) {

}

#include "pq.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
    uint32_t head;
    uint32_t capacity;
    Node **items;
};

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->head = 0;
        q->capacity = capacity;
        q->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}

void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
    return;
}

bool pq_empty(PriorityQueue *q) {
    if (q->head == 0) {
        return true;
    }
    return false;
}

bool pq_full(PriorityQueue *q) {
    if (q->head == q->capacity) {
        return true;
    }
    return false;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->head;
}

void insertion_sort(PriorityQueue *q) {
    Node *temp_item;
    for (uint32_t index = 0; index < q->head; index++) {
        uint32_t compare_index = index;
        temp_item = q->items[index];
        while ((compare_index > 0)
               && (temp_item->frequency > q->items[compare_index - 1]->frequency)) {
            q->items[compare_index] = q->items[compare_index - 1];
            compare_index -= 1;
        }
        q->items[compare_index] = temp_item;
    }
}
bool enqueue(PriorityQueue *q, Node *n) {
    if (q->head < q->capacity) {
        q->items[q->head] = n;
        q->head++;
        insertion_sort(q);
        return true;
    }
    return false;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (q->head > 0) {
        q->head--;
        *n = q->items[q->head];
        return true;
    }
    return false;
}

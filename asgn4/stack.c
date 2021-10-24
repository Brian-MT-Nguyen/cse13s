#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s) == false) {
        s->items[s->top] = x;
        s->top += 1;
        return true;
    }
    return false;
}

bool stack_pop(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        *x = s->items[s->top];
        s->top -= 1;
        return true;
    }
    return false;
}

bool stack_peek(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        *x = s->items[s->top];
        return true;
    }
    return false;
}

void stack_copy(Stack *dst, Stack *src) {
    for (uint32_t i = 0; i < src->capacity; i++) {
        dst->items[i] = src->items[i];
    }
}

void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}

#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//Variables that make up the definition of the stack ADT (or its structure)
struct Stack {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};

//Creates the a dynamically allocated array of the Stack based on inputs
//Returns the array of the stack
//
//capacity: the number of items that will be in the stack
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

//Frees the stack array and its items then nulls the memory out
//
//s: the stack
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

//Shows whether the stack is empty based on if the top is zero
//Returns true if top is zero making stack empty
//Returns false if top is not zero making stack contain something
//
//s: the stack
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}
//Shows whether the stack is full based on if the top is capacity
//Returns true if top is equal to capacity making stack full
//Returns false if top is not equal to capacity making it less than full
//
//s: the stack
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

//Gives the size of the stack based on where the index slot top is at
//Returns the size of the stack
//
//s: the stack
uint32_t stack_size(Stack *s) {
    return s->top;
}

//Pushes or adds an item onto the stack if the stack is not full
//Returns true if the item was pushed
//Returns false if the item could not be pushed
//
//s: the stack
//x: the item to push
bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s) == false) {
        s->items[s->top] = x;
        s->top += 1;
        return true;
    }
    return false;
}

//Pops or removes an item from the stack if the stack is not empty
//Returns true if the item was popped
//Returns false if the item could not be popped
//
//s: the stack
//x: the place to store the popped item
bool stack_pop(Stack *s, uint32_t *x) {
    if (stack_empty(s) == false) {
        s->top -= 1;
        *x = s->items[s->top];
        return true;
    }
    return false;
}

//Gives the item at the top of the stack based on top
//Returns true if there is an item on the stack
//Returns fale if the stack is empty and there is no item
//
//s: the stack
//x: the place to store the item checked
bool stack_peek(Stack *s, uint32_t *x) {
    if (stack_empty(s) == false) {
        *x = s->items[s->top - 1];
        return true;
    }
    return false;
}

//Copies the items of the stack from its source to another destination
//
//dst: the place to copy the items of the stack to
//src: the source of the items of the stack
void stack_copy(Stack *dst, Stack *src) {
    for (uint32_t i = 0; i < src->capacity; i++) {
        dst->items[i] = src->items[i];
    }
    dst->top = src->top;
}

//Prints the cities of the path to an outfile for output
//
//s: the stack
//outfile: the file to print output to
//cities: the array of cities that make up the path
void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}

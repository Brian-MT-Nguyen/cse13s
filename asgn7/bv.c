#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        uint32_t bytes = length / 8 + (length % 8 ? 1 : 0);
        bv->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
    }
    return bv;
}

void bv_delete(BitVector **bv) {
    if (*bv) {
        if ((*bv)->vector) {
            free((*bv)->vector);
        }
        free(*bv);
        *bv = NULL;
    }
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

bool bv_set_bit(BitVector *bv, uint32_t i) {
    if ((i < bv->length) && bv && bv->vector) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    }
    return false;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if ((i < bv->length) && bv && bv->vector) {
        bv->vector[i / 8] &= ~(0x1 << i % 8);
        return true;
    }
    return false;
}

bool bv_get_bit(BitVector *bv, uint32_t i) {
    if ((i < bv->length) && bv && bv->vector) {
        uint8_t copy_byte = bv->vector[i / 8];
        uint8_t get_bit = 0x1 & (copy_byte >> i % 8);
        if (get_bit == 1) {
            return true;
        }
    }
    return false;
}

void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i++) {
        if (i % 8 == 0) {
            printf("#%d: %d\n", i / 8, bv->vector[i / 8]);
        }
    }
}

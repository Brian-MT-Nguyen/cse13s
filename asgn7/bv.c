#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

//Dynamically allocates an array of bytes to hold at least length bits and sets length
//Returns the bit vector
//
//length: the length of bits the vector will hold
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        uint32_t bytes = length / 8 + (length % 8 ? 1 : 0);
        bv->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
    }
    return bv;
}

//Frees memory of all bytes in vector then the vector itself
//
//bv: the bit vector
void bv_delete(BitVector **bv) {
    if (*bv) {
        if ((*bv)->vector) {
            free((*bv)->vector);
        }
        free(*bv);
        *bv = NULL;
    }
}

//Returns the length in bits of the bit vector
//
//bv: the bit vector
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

//Sets the ith bit to 1 if in range
//Returns true if in range and added
//Returns false if not in range and not added
//
//bv: the bit vector
//i: the index
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if ((i < bv->length) && bv && bv->vector) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    }
    return false;
}

//Sets the ith bit to 0 (clears it) if in range
//Returns true if in range and cleared
//Returns false if not in range and not cleared
//
//bv: the bit vector
//i: the index
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if ((i < bv->length) && bv && bv->vector) {
        bv->vector[i / 8] &= ~(0x1 << i % 8);
        return true;
    }
    return false;
}

//Gets the ith bit of the vector if in range
//Returns true if the bit is 1
//Returns false if the bit is 0
//
//bv: the bit vector
//i: the index
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

//Debug function to print the bytes of the vector
//
//bv: the bit vector
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i++) {
        if (i % 8 == 0) {
            printf("#%d: %d\n", i / 8, bv->vector[i / 8]);
        }
    }
}

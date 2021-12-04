#include "bf.h"
#include "salts.h"
#include "speck.h"
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt.
    uint64_t secondary[2]; // Secondary hash function salt.
    uint64_t tertiary[2]; // Tertiary hash function salt.
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;

        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;

        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;

        bf->filter = bv_create(size);
    }
    return bf;
}

void bf_delete(BloomFilter **bf) {
    if (*bf) {
        if ((*bf)->filter) {
            free((*bf)->filter);
        }
        free(*bf);
        *bf = NULL;
    }
}

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t pri_index = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t sec_index = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t ter_index = hash(bf->tertiary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, pri_index);
    bv_set_bit(bf->filter, sec_index);
    bv_set_bit(bf->filter, ter_index);
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t pri_index = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t sec_index = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t ter_index = hash(bf->tertiary, oldspeak) % bf_size(bf);
    if (bv_get_bit(bf->filter, pri_index) && bv_get_bit(bf->filter, sec_index)
        && bv_get_bit(bf->filter, ter_index)) {
        return true;
    }
    return false;
}

uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}

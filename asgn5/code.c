#include "code.h"
#include <stdio.h>

Code code_init(void) {
	Code c;
	c.top = 0;
	for(uint8_t i = 0; i < MAX_CODE_SIZE; i++) {
		c.bits[i]= 0x0;
	}
	return c;
}

uint32_t code_size(Code *c) {
	return c->top;
}

bool code_empty(Code *c) {
	if(c->top == 0) {
		return true;
	}
	return false;
}

bool code_full(Code *c) {
	if(c-> top == MAX_CODE_SIZE) {
		return true;
	}
	return false;
}

bool code_set_bit(Code *c, uint32_t i) {
	if (i >= 0 && i < ALPHABET) {
		uint8_t set_bit = 0x00;
        	set_bit = (0x1 << i%8);
		c->bits[i/8] |= set_bit;
		return true;
	}
	return false;
	
}

bool code_clr_bit(Code *c, uint32_t i) {
	if (i >= 0 && i < ALPHABET) {
                uint8_t clr_bit = 0x00;
                clr_bit = (0x1 << i%8);
		clr_bit = ~clr_bit;
                c->bits[i/8] &= clr_bit;
                return true;
        }
        return false;	
}

bool code_get_bit(Code *c, uint32_t i) {
	if (i >= 0 && i < ALPHABET) {
                uint8_t get_bit = 1 & (c->bits[i/8] >> i%8);
		if (get_bit == 1) {
			return true;
		}
        }
        return false;	
}

bool code_push_bit(Code *c, uint8_t bit) {
	if (c -> top < MAX_CODE_SIZE) {
		if(bit == 1) {
			code_set_bit(c, c-> top);
		}
		else {
			code_clr_bit(c, c-> top);
		}
		c -> top++;
		return true;
	}
	return false;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
	if (c) {
		*bit = code_get_bit(c, c->top);
		return true;
	}
	return false;
}

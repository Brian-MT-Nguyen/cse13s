#include "io.h"
#include "code.h"
#include "header.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

uint64_t bytes_read;
uint64_t bytes_written;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int file_bytes_read = 0;
    int buffer_bytes = 1;

    while ((nbytes > file_bytes_read) && (buffer_bytes > 0)) {
        buffer_bytes = read(infile, buf, nbytes);
        file_bytes_read += buffer_bytes;
    }

    bytes_read += file_bytes_read;
    return file_bytes_read;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int file_bytes_written = 0;
    int buffer_bytes = 1;

    while ((nbytes > file_bytes_written) && (buffer_bytes > 0)) {
        buffer_bytes = write(outfile, buf, nbytes);
        file_bytes_written += buffer_bytes;
    }
    bytes_written += file_bytes_written;
    return file_bytes_written;
}

bool read_bit(int infile, uint8_t *bit) {
    static uint8_t buffer[BLOCK];
    static int index = 0;
    static int end = -1;
    uint8_t buffer_copy;

    if (index == 0) {
        int buffer_bytes = read_bytes(infile, buffer, BLOCK);
        if (buffer_bytes < BLOCK) {
            end = buffer_bytes * 8 + 1;
        }
    }
    buffer_copy = buffer[index / 8];
    *bit = (buffer_copy >> index % 8) & 1;
    index += 1;

    if (index == BLOCK * 8) {
        index = 0;
    }

    return index != end / 8;
}

static uint8_t shared_buffer[BLOCK];
static int index = 0;

void write_code(int outfile, Code *c) {
    for (uint8_t i = 0; i < code_size(c); i++) {
        uint8_t bit = code_get_bit(c, i);
        if (bit == 1) {
            code_set_bit(c, index);
        }
        if (bit == 0) {
            code_clr_bit(c, index);
        }
        index++;
        if (index == BLOCK) {
            flush_codes(outfile);
            index = 0;
        }
    }
}

void flush_codes(int outfile) {
    if (index > 0) {
        index /= 8;
        write_bytes(outfile, shared_buffer, BLOCK);
    }
}



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "MD4.h"

#define a0_md4 0x67452301
#define b0_md4 0xefcdab89
#define c0_md4 0x98badcfe
#define d0_md4 0x10325476


const int s1_md4[4] = {
    3, 7, 11, 19
};

const int s2_md4[4] = {
    3, 5, 9, 13
};

const int s3_md4[4] = {
    3, 9, 11, 15
};

const int z1_md4[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

const int z2_md4[16] = {
    0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15
};

const int z3_md4[16] = {
    0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15
};

const uint32_t y_md4[3] = {
    0, 0x5a827999, 0x6ed9eba1
};


typedef struct Block {
    uint32_t data[16];
} Block;

typedef struct Blocks {
    Block *blocks;
    uint32_t size;
} Blocks;

typedef struct Message {
	uint8_t *message;
	uint64_t size;
} Message;

typedef struct Hash {
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
} Hash;


uint32_t f_md4(uint32_t b, uint32_t c, uint32_t d) {
    return (b & c) | (~b & d);
}

uint32_t g_md4(uint32_t b, uint32_t c, uint32_t d) {
    return (b & c) | (b & d) | (c & d);
}

uint32_t h_md4(uint32_t b, uint32_t c, uint32_t d) {
    return b ^ c ^ d;
}

uint32_t rotateLeft_md4(uint32_t num, int count) {
    return (num << count) | (num >> (32 - count));
}


void ff_md4(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + f_md4(*b, *c, *d) + block.data[z1_md4[i]] + y_md4[0];
        result = rotateLeft_md4(result, s1_md4[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void gg_md4(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + g_md4(*b, *c, *d) + block.data[z2_md4[i]] + y_md4[1];
        result = rotateLeft_md4(result, s2_md4[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void hh_md4(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + h_md4(*b, *c, *d) + block.data[z3_md4[i]] + y_md4[2];
        result = rotateLeft_md4(result, s3_md4[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void extractHash_md4(Hash *hash, char *result) {
    uint32_t a = 0, b = 0, c = 0, d = 0;
    a = (hash->A << 24) | ((hash->A << 8) & 0x00ff0000) | ((hash->A >> 8) & 0x0000ff00) | (hash->A >> 24);
    b = (hash->B << 24) | ((hash->B << 8) & 0x00ff0000) | ((hash->B >> 8) & 0x0000ff00) | (hash->B >> 24);
    c = (hash->C << 24) | ((hash->C << 8) & 0x00ff0000) | ((hash->C >> 8) & 0x0000ff00) | (hash->C >> 24);
    d = (hash->D << 24) | ((hash->D << 8) & 0x00ff0000) | ((hash->D >> 8) & 0x0000ff00) | (hash->D >> 24);

    snprintf(result, 129, "%08x%08x%08x%08x", a, b, c, d);
}


Hash *hash_md4(Blocks *blocks) {
    uint32_t a = a0_md4, b = b0_md4, c = c0_md4, d = d0_md4;

    for (int i = 0; i < blocks->size; i++) {
        ff_md4(&a, &b, &c, &d, blocks->blocks[i]);
        gg_md4(&a, &b, &c, &d, blocks->blocks[i]);
        hh_md4(&a, &b, &c, &d, blocks->blocks[i]);
    }

    Hash *hash = calloc(1, sizeof(Hash));
    hash->A = a + a0_md4;
    hash->B = b + b0_md4;
    hash->C = c + c0_md4;
    hash->D = d + d0_md4;

    return hash;
}


Blocks *convertMessage_md4(const Message *message) {
    Blocks *blocks = calloc(1, sizeof(Blocks));
    blocks->size = message->size / 64;
    blocks->blocks = calloc(blocks->size, sizeof(Block));

    for (int i = 0; i < blocks->size; i++) {
        Block block;
        uint32_t index = i * 64;

        for (int j = 0; j < 16; j++) {
            uint32_t word = 0;

            for (int k = 0; k < 4; k++) {
                uint32_t tmp = message->message[index++];
                word |= tmp << (k * 8);
            }

            block.data[j] = word;
        }

        blocks->blocks[i] = block;
    }

    return blocks;
}


Message *extractMessage_md4(const char *message, const uint64_t messageLength) {
    uint8_t *result = calloc(messageLength, sizeof(uint8_t));
    uint64_t resultSize = messageLength;

    for (uint64_t i = 0; message[i] != '\0'; i++) {
        result[i] = message[i];
    }

    uint64_t paddingSize = 64 - (messageLength % 64);

    if (paddingSize > 8) {
        result = realloc(result, sizeof(uint8_t) * (resultSize + paddingSize));
        result[resultSize++] = 0x80;
        paddingSize--;

        while (paddingSize-- > 8) {
            result[resultSize++] = 0x00;
        }
    } else {
        result = realloc(result, sizeof(uint8_t) * (resultSize + paddingSize + 64));
        result[resultSize++] = 0x80;
        paddingSize += 64 - 1;

        while (paddingSize-- > 8) {
            result[resultSize++] = 0x00;
        }
    }

    uint64_t lenght = messageLength * 8;

    for (int i = 0; i < 8; i++) {
        result[resultSize++] = (lenght >> (i * 8)) & 0x000000ff;
    }

    Message *tmp = calloc(1, sizeof(Message));
    tmp->message = result;
    tmp->size = resultSize;

    return tmp;
}


/**
 * @brief Digest message and hash it, then return the resulting 128-bit hash to the result
 * 
 * @param message The message string
 * @param messageLength The message string length
 * @param result The result string to appointed to. Must be allocated with 129 spaces.
 */
void digest_md4(char *message, uint64_t messageLength, char *result) {
    Message *messagePadding = extractMessage_md4(message, messageLength);
    Blocks *blocks = convertMessage_md4(messagePadding);
    Hash *hash = hash_md4(blocks);

    extractHash_md4(hash, result);
    free(messagePadding);
    free(blocks);
    free(hash);
}

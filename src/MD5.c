#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "MD5.h"

#define a0_md5 0x67452301
#define b0_md5 0xefcdab89
#define c0_md5 0x98badcfe
#define d0_md5 0x10325476


const int z1_md5[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

const int z2_md5[16] = {
    1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12
};

const int z3_md5[16] = {
    5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2
};

const int z4_md5[16] = {
    0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
};

const int s1_md5[4] = {
    7, 12, 17, 22
};

const int s2_md5[4] = {
    5, 9, 14, 20
};

const int s3_md5[4] = {
    4, 11, 16, 23
};

const int s4_md5[4] = {
    6, 10, 15, 21
};

const uint32_t y_md5[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
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


uint32_t f_md5(uint32_t B, uint32_t C, uint32_t D) {
    return (B & C) | (~B & D);
}

uint32_t g_md5(uint32_t B, uint32_t C, uint32_t D) {
    return (B & D) | (C & ~D);
}

uint32_t h_md5(uint32_t B, uint32_t C, uint32_t D) {
    return B ^ C ^ D;
}

uint32_t i_md5(uint32_t B, uint32_t C, uint32_t D) {
    return C ^ (B | ~D);
}

uint32_t rotateLeft_md5(uint32_t num, int count) {
    return (num << count) | (num >> (32 - count));
}


void ff_md5(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block_MD5) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + f_md5(*b, *c, *d) + block_MD5.data[z1_md5[i]] + y_md5[i];
        result = *b + rotateLeft_md5(result, s1_md5[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void gg_md5(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block_MD5) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + g_md5(*b, *c, *d) + block_MD5.data[z2_md5[i]] + y_md5[i + 16];
        result = *b + rotateLeft_md5(result, s2_md5[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void hh_md5(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block_MD5) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + h_md5(*b, *c, *d) + block_MD5.data[z3_md5[i]] + y_md5[i + 32];
        result = *b + rotateLeft_md5(result, s3_md5[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void ii_md5(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block_MD5) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + i_md5(*b, *c, *d) + block_MD5.data[z4_md5[i]] + y_md5[i + 48];
        result = *b + rotateLeft_md5(result, s4_md5[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void extractHash_md5(Hash *hash, char *result) {
    uint32_t a = 0, b = 0, c = 0, d = 0;
    a = (hash->A << 24) | ((hash->A << 8) & 0x00ff0000) | ((hash->A >> 8) & 0x0000ff00) | (hash->A >> 24);
    b = (hash->B << 24) | ((hash->B << 8) & 0x00ff0000) | ((hash->B >> 8) & 0x0000ff00) | (hash->B >> 24);
    c = (hash->C << 24) | ((hash->C << 8) & 0x00ff0000) | ((hash->C >> 8) & 0x0000ff00) | (hash->C >> 24);
    d = (hash->D << 24) | ((hash->D << 8) & 0x00ff0000) | ((hash->D >> 8) & 0x0000ff00) | (hash->D >> 24);

    snprintf(result, 129, "%08x%08x%08x%08x", a, b, c, d);
}


Hash *hash_md5(Blocks *blocks) {
    uint32_t a = a0_md5, b = b0_md5, c = c0_md5, d = d0_md5;

    for (int i = 0; i < blocks->size; i++) {
        ff_md5(&a, &b, &c, &d, blocks->blocks[i]);
        gg_md5(&a, &b, &c, &d, blocks->blocks[i]);
        hh_md5(&a, &b, &c, &d, blocks->blocks[i]);
        ii_md5(&a, &b, &c, &d, blocks->blocks[i]);
    }

    Hash *hash = calloc(1, sizeof(Hash));
    hash->A = a + a0_md5;
    hash->B = b + b0_md5;
    hash->C = c + c0_md5;
    hash->D = d + d0_md5;

    return hash;
}


Blocks *convertMessage_md5(const Message *message) {
    Blocks *blocks = calloc(1, sizeof(Blocks));
    blocks->size = message->size / 64;
    blocks->blocks = calloc(blocks->size, sizeof(Block));

    for (int i = 0; i < blocks->size; i++) {
        Block block_MD5;
        uint32_t index = i * 64;

        for (int j = 0; j < 16; j++) {
            uint32_t word = 0;

            for (int k_MD5 = 0; k_MD5 < 4; k_MD5++) {
                uint32_t tmp = message->message[index++];
                word |= tmp << (k_MD5 * 8);
            }

            block_MD5.data[j] = word;
        }

        blocks->blocks[i] = block_MD5;
    }

    return blocks;
}


Message *extractMessage_md5(const char *message, const uint64_t messageLength) {
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


uint64_t getMessageLength_md5(const char *message) {
    uint64_t length = 0;

    for (uint64_t i = 0; message[i] != '\0'; i++) {
        length++;
    }

    return length;
}


/**
 * @brief Digest message and hash it, then return the resulting 128-bit hash to the result
 * 
 * @param message The message string
 * @param messageLength The message string length
 * @param result The result string to appointed to. Must be allocated with 129 spaces.
 */
void digest_md5(const char *message, char *result) {
    uint64_t messageLength = getMessageLength_md5(message);
    Message *messagePadding = extractMessage_md5(message, messageLength);
    Blocks *blocks = convertMessage_md5(messagePadding);
    Hash *hash = hash_md5(blocks);

    extractHash_md5(hash, result);
    free(messagePadding);
    free(blocks);
    free(hash);
}

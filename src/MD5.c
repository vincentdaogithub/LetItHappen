#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "MD5.h"

#define A_MD5 0x67452301
#define B_MD5 0xefcdab89
#define C_MD5 0x98badcfe
#define D_MD5 0x10325476


uint32_t S1[4] = {
    7, 12, 17, 22
};

uint32_t S2[4] = {
    5, 9, 14, 20
};

uint32_t S3[4] = {
    4, 11, 16, 23
};

uint32_t S4[4] = {
    6, 10, 15, 21
};

uint32_t K[64] = {
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


uint32_t F(uint32_t B, uint32_t C, uint32_t D) {
    return (B & C) | (~B & D);
}

uint32_t G(uint32_t B, uint32_t C, uint32_t D) {
    return (B & D) | (C & ~D);
}

uint32_t H(uint32_t B, uint32_t C, uint32_t D) {
    return B ^ C ^ D;
}

uint32_t I(uint32_t B, uint32_t C, uint32_t D) {
    return C ^ (B | ~D);
}

uint32_t rotateLeft(uint32_t num, int count) {
    return (num << count) | (num >> (32 - count));
}


void FF(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block) {
    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + F(*b, *c, *d) + block.data[i] + K[i];
        result = *b + rotateLeft(result, S1[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;
    }
}


void GG(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block) {
    int index = 1;

    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + G(*b, *c, *d) + block.data[index] + K[i + 16];
        result = *b + rotateLeft(result, S2[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;

        index = (index + 5) % 16;
    }
}


void HH(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block) {
    int index = 5;

    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + H(*b, *c, *d) + block.data[index] + K[i + 32];
        result = *b + rotateLeft(result, S3[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;

        index = (index + 3) % 16;
    }
}


void II(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, Block block) {
    int index = 0;

    for (int i = 0; i < 16; i++) {
        uint32_t result = *a + I(*b, *c, *d) + block.data[index] + K[i + 48];
        result = *b + rotateLeft(result, S4[i % 4]);
        *a = *d;
        *d = *c;
        *c = *b;
        *b = result;

        index = (index + 7) % 16;
    }
}


void extractHash(Hash *hash, char *result) {
    uint32_t a = 0, b = 0, c = 0, d = 0;
    a = (hash->A << 24) | ((hash->A << 8) & 0x00ff0000) | ((hash->A >> 8) & 0x0000ff00) | (hash->A >> 24);
    b = (hash->B << 24) | ((hash->B << 8) & 0x00ff0000) | ((hash->B >> 8) & 0x0000ff00) | (hash->B >> 24);
    c = (hash->C << 24) | ((hash->C << 8) & 0x00ff0000) | ((hash->C >> 8) & 0x0000ff00) | (hash->C >> 24);
    d = (hash->D << 24) | ((hash->D << 8) & 0x00ff0000) | ((hash->D >> 8) & 0x0000ff00) | (hash->D >> 24);

    snprintf(result, 129, "%08x%08x%08x%08x", a, b, c, d);
}


Hash *hashMD5(Blocks *blocks) {
    uint32_t *a = calloc(1, sizeof(uint32_t));
    *a = A_MD5;

    uint32_t *b = calloc(1, sizeof(uint32_t));
    *b = B_MD5;

    uint32_t *c = calloc(1, sizeof(uint32_t));
    *c = C_MD5;

    uint32_t *d = calloc(1, sizeof(uint32_t));
    *d = D_MD5;

    for (int i = 0; i < blocks->size; i++) {
        FF(a, b, c, d, blocks->blocks[i]);
        GG(a, b, c, d, blocks->blocks[i]);
        HH(a, b, c, d, blocks->blocks[i]);
        II(a, b, c, d, blocks->blocks[i]);
    }

    Hash *hash = calloc(1, sizeof(Hash));
    hash->A = *a + A_MD5;
    hash->B = *b + B_MD5;
    hash->C = *c + C_MD5;
    hash->D = *d + D_MD5;

    free(a);
    free(b);
    free(c);
    free(d);
    return hash;
}


Blocks *convertMessage(const Message *message) {
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


Message *extractMessage(const char *message, const uint64_t messageLength) {
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


void digestMD5(char *message, uint64_t messageLength, char *result) {
    Message *messagePadding = extractMessage(message, messageLength);
    Blocks *blocks = convertMessage(messagePadding);
    Hash *hash = hashMD5(blocks);

    extractHash(hash, result);
    free(messagePadding);
    free(blocks);
    free(hash);
}

#ifndef MD5_H
#define MD5_H
#define HASH_LENGTH 128

#include <stdint.h>

typedef struct MD5 {
    uint64_t length;
} MD5;

void digestMD5(char* message, uint64_t messageLength, char *result);

#endif MD5_H    // MD5_H
#ifndef MD5_H
#define MD5_H
#define HASH_LENGTH 128

#include <stdint.h>

/**
 * @brief Digest message and hash it, then return the resulting 128-bit hash to the result
 * 
 * @param message The message string
 * @param messageLength The message string length
 * @param result The result string to appointed to. Must be allocated with 129 spaces.
 */
void digestMD5(char* message, uint64_t messageLength, char *result);

#endif MD5_H // MD5_H
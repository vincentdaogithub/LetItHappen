#ifndef MD5
#define MD5

#include <stdint.h>

/**
 * @brief Digest message and hash it in MD5, then return the resulting 128-bit hash to the result
 * 
 * @param message The message string
 * @param messageLength The message string length
 * @param result The result string to appointed to. Must be allocated with 129 spaces.
 */
void digest_md5(const char* message, char *result);

#endif  // MD5
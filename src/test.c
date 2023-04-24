#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MD4.h"
#include "MD5.h"


void testMD5() {
	printf("Begin test for MD5:\n");
    char *result = calloc(129, sizeof(char));

    char *test1 = "1";
    digest_md5(test1, result);
    printf("Test for \"%s\": %s\n", test1, !strcmp(result, "c4ca4238a0b923820dcc509a6f75849b") ? "ok" : "nope");
    printf("%s\n\n", result);

    char *test2 = "The quick brown fox jumps over the lazy dog";
    digest_md5(test2, result);
    printf("Test for \"%s\": %s\n", test2, !strcmp(result, "9e107d9d372bb6826bd81d3542a419d6") ? "ok" : "nope");
    printf("%s\n\n", result);

    char *test3 = "Ad astra per aspera";
    digest_md5(test3, result);
    printf("Test for \"%s\": %s\n", test3, !strcmp(result, "17f22b77464f057d6dcd0f32a0fd1b80") ? "ok" : "nope");
    printf("%s\n\n", result);
    printf("------------------------------------------\n");
}

void testMD4() {
	printf("Begin test for MD4:\n");
    char *result = calloc(129, sizeof(char));

    char *test1 = "1";
    digest_md4(test1, result);
    printf("Test for \"%s\": %s\n", test1, !strcmp(result, "8be1ec697b14ad3a53b371436120641d") ? "ok" : "nope");
    printf("%s\n\n", result);

    char *test2 = "The quick brown fox jumps over the lazy dog";
    digest_md4(test2, result);
    printf("Test for \"%s\": %s\n", test2, !strcmp(result, "1bee69a46ba811185c194762abaeae90") ? "ok" : "nope");
    printf("%s\n\n", result);

    char *test3 = "";
    digest_md4(test3, result);
    printf("Test for \"%s\": %s\n", test3, !strcmp(result, "31d6cfe0d16ae931b73c59d7e0c089c0") ? "ok" : "nope");
    printf("%s\n\n", result);
    printf("------------------------------------------\n");
}

int main(int argc, char **argv) {
    testMD5();
    testMD4();
    return EXIT_SUCCESS;
}

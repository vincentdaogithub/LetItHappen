#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char *result = calloc(129, sizeof(char));

    char *test1 = "1";
    digestMD5(test1, 1, result);
    printf("Test for \"%s\": %s\n", test1, !strcmp(result, "c4ca4238a0b923820dcc509a6f75849b") ? "ok" : "nope");
    printf("%s\n\n", result);

    char *test2 = "The quick brown fox jumps over the lazy dog";
    digestMD5(test2, 43, result);
    printf("Test for \"%s\": %s\n", test2, !strcmp(result, "9e107d9d372bb6826bd81d3542a419d6") ? "ok" : "nope");
    printf("%s\n\n", result);

    char *test3 = "";
    digestMD5(test3, 0, result);
    printf("Test for \"%s\": %s\n", test3, !strcmp(result, "d41d8cd98f00b204e9800998ecf8427e") ? "ok" : "nope");
    printf("%s\n\n", result);

    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
#include <clasp/target.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: dll_test lib\n");
        return 1;
    }

    char *filename = argv[1];
    _DLL *lib = _DLL_open(filename);
    int (*add)(int a, int b) = _DLL_load(lib, int, "add");

    int a = 5;
    int b = 3;
    printf("%d + %d = %d\n", a, b, add(a,b));

    return 0;
}
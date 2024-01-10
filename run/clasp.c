#include <clasp/clasp.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Failed to open file %s\n", filename);
        return -1;
    }
}
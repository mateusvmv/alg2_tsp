#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[argc]) {
    assert(argc == 3);
    FILE *out = fopen(argv[1], "w");
    int n;
    assert(sscanf(argv[2], "%d", &n) == 1);
    fprintf(
        out,
        "NAME : rand%1$d\n"
        "COMMENT : %1$d random cities TSP\n"
        "TYPE : TSP\n"
        "DIMENSION : %1$d\n"
        "EDGE_WEIGHT_TYPE : EUC_2D\n"
        "NODE_COORD_SECTION\n",
        n
    );
    for(int i = 0; i < n; i += 1)
        fprintf(out, "%d %lf %lf\n", i+1, rand()*0.0001, rand() * 0.0001);
    fprintf(out, "EOF\n");
    fclose(out);
}

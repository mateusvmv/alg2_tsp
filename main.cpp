#include <assert.h>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lemon/list_graph.h>
#include <lemon/matching.h>
#include <lemon/euler.h>

double calculate_cost(int n, double (*coords)[2], int *tsp);

// Algoritmo de Prim para Árvore Geradora Mínima
// Recebe n, o número de vértices, e coords, uma array de n coordenadas
// Coloca o resultado na array par, que deve ter capacidade para n elementos
// par[i] = j diz que o pai do vértice i na árvore geradora mínima é o vértice j
// A função roda em O(n^2), com O(n) de memória.
// As arestas são implícitas pela coordenada, por isso O(n^2) não entra no uso de memória.
void prim(int n, double (*coords)[2], int *par) {
    double *small = new double[n];
    memset(par, 0, n * sizeof(int));
    memset(small, ~0, n * sizeof(int));
    small[0] = NAN;
    for(int j = 1; j < n; j += 1)
        small[j] = hypot(coords[0][0] - coords[j][0], coords[0][1] - coords[j][1]);
    for(int i = 1; i < n; i += 1) {
        int k = -1;
        for(int j = 0; j < n; j += 1)
            if(!isnan(small[j]) && (k == -1 || small[j] < small[k])) k = j;
        small[k] = NAN;
        for(int j = 0; j < n; j += 1) {
            double d = hypot(coords[k][0] - coords[j][0], coords[k][1] - coords[j][1]);
            if(!isnan(small[j]) && d < small[j])
                par[j] = k, small[j] = d;
        }
    }
    for(int i = 0; i < n; i += 1) assert(isnan(small[i]));
    for(int i = 1; i < n; i += 1) assert(par[i] != i);
    delete[] small;
}

// Retorna listas de crianças para cada vértice da árvore
// Crianças de i estão entre child[i] (incluso) e child[i+1] (excluso)
// Portanto, child precisa de ter capacidade n+1
// E data precisad e ter capacidade n e armazena os dados apontados por child[i]
// A função roda em O(n)
void tree_children(int n, int *par, int **child, int *data) {
    memset(data, 0, n * sizeof(int));
    for(int i = 0; i < n; i += 1)
        data[par[i]] += 1;
    child[0] = data + data[0];
    for(int i = 1; i < n; i += 1)
        child[i] = child[i-1] + data[i];
    child[n] = child[n-1];
    for(int i = n-1; i > 0; i -= 1)
        *--child[par[i]] = i;
    for(int i = 0; i < n; i += 1)
        for(int *c = child[i], *e = child[i+1]; c < e; c += 1)
            assert(par[*c] == i);
}

// Algoritmo Twice around the Tree
// Recebe um número de vértices n, e n coordenadas
// O resultado é colocado na array tsp, que deve ter capacidade n
// A função roda em O(n^2) e usa O(n) de memória
void tatt(int n, double (*coords)[2], int *tsp) {
    memset(tsp, 0, n * sizeof(int));

    int *par = new int[n];
    prim(n, coords, par);

    int (**child) = new int*[n+1];
    int *cd = new int[n];
    tree_children(n, par, child, cd);

    int **pos = new int*[n];
    memset(pos, 0, n * sizeof(int*));
    int i = 0, k = 0;
    while(1) {
        if(!pos[i]) {
            pos[i] = child[i];
            tsp[k++] = i;
        }
        if(pos[i] < child[i+1]) {
            i = *(pos[i]++);
        } else {
            if(i == 0) break;
            i = par[i];
        }
    }

    delete[] pos;
    delete[] par;
    delete[] child;
    delete[] cd;
}

// Algoritmo Twice around the Tree
// Recebe um número de vértices n, e n coordenadas
// O resultado é colocado na array tsp, que deve ter capacidade n
// A função roda em O(n^4) e usa O(n^2) de memória
// Por causa do uso da lib lemon, não foi possível usar O(n) de memória aqui
// As arestas tiveram de ser explícitas
void christofides(int n, double (*coords)[2], int *tsp) {
    memset(tsp, 0, n * sizeof(int));

    int *par = new int[n];
    prim(n, coords, par);

    int (**child) = new int*[n+1];
    int *cd = new int[n];
    tree_children(n, par, child, cd);

    int *odd_bgn = new int[n], *odd_end = odd_bgn;
    if((child[1] - child[0]) % 2 == 1)
        *(odd_end++) = 0;
    for(int i = 1; i < n; i += 1)
        if((child[i+1] - child[i]) % 2 == 0)
            *(odd_end++) = i;

    int m = odd_end - odd_bgn;
    lemon::ListGraph lg;
    lemon::ListGraph::EdgeMap<double> lgem(lg);
    lg.reserveNode(m);
    lg.reserveEdge(m * (m + 1) / 2);
    for(int i = 0; i < m; i += 1) lg.addNode();
    double max_dist = 0;
    for(int i = 0; i < m; i += 1)
        for(int j = i+1; j < m; j += 1) {
            int a = odd_bgn[i], b = odd_bgn[j];
            double d = hypot(coords[a][0] - coords[b][0], coords[a][1] - coords[b][1]);
            max_dist = std::max(max_dist, d);
            auto na = lg.nodeFromId(i);
            auto nb = lg.nodeFromId(j);
            auto e = lg.addEdge(na, nb);
            lgem.set(e, -d);
        }

    lemon::MaxWeightedPerfectMatching<lemon::ListGraph, lemon::ListGraph::EdgeMap<double>>
        mwpm(lg, lgem);
    mwpm.run();

    lemon::ListGraph united;
    for(int i = 0; i < n; i += 1) united.addNode();
    for(int i = 0; i < m; i += 1) {
        int j = lg.id(mwpm.mate(lg.nodeFromId(i)));
        int a = odd_bgn[i], b = odd_bgn[j];
        united.addEdge(united.nodeFromId(a), united.nodeFromId(b));
    }
    for(int i = 1; i < n; i += 1) {
        int kappa = united.maxEdgeId();
        united.addEdge(united.nodeFromId(i), united.nodeFromId(par[i]));
        assert(united.maxEdgeId() == kappa + 1);
    }

    lemon::EulerIt<lemon::ListGraph> eit(united);
    bool *added = new bool[n];
    memset(added, 0, n * sizeof(bool));
    int k = 0;
    for(; eit != lemon::INVALID; ++eit) {
        lemon::ListGraph::Edge e(eit);
        int v = united.id(united.v(e));
        if(added[v]) continue;
        added[v] = 1;
        tsp[k++] = v;
    }

    delete[] added;
    delete[] odd_bgn;
    delete[] par;
    delete[] child;
    delete[] cd;
}

// Recursão do Branch and Bound
// Exponencial, O(n) a cada chamada
// O cálculo dos limites inferior e superior é bem optimizado e constante
// O uso de memória é O(1), dessa função.
void branch_and_bound_recur(
    int i,
    int n,
    double (*coords)[2],
    bool *vis,
    double *gmax,
    const double (*most)[2],
    const double (*least)[2],
    double min,
    double max,
    int *tsp,
    int *best
) {
    if(i == n) {
        int a = tsp[i-1], b = tsp[0];
        double d = hypot(coords[a][0] - coords[b][0], coords[a][1] - coords[b][1]);
        min = min - (least[a][0] + least[b][1]) / 2 + d;
        max = max - (most[a][0] + most[b][1]) / 2 + d;
    }
    if(min >= *gmax) return;
    *gmax = std::min(*gmax, max);
    if(i == n) {
        memcpy(best, tsp, n * sizeof(int));
        return;
    }
    int a = tsp[i-1];
    for(int b = 0; b < n; b += 1) {
        if(vis[b]) continue;
        vis[b] = 1;
        tsp[i] = b;
        double d = hypot(coords[a][0] - coords[b][0], coords[a][1] - coords[b][1]);
        double nmin = min - (least[a][0] + least[b][1]) / 2 + d;
        double nmax = max - (most[a][0] + most[b][1]) / 2 + d;
        branch_and_bound_recur(
            i + 1,
            n,
            coords,
            vis,
            gmax,
            most,
            least,
            nmin,
            nmax,
            tsp,
            best
        );
        vis[b] = 0;
    }
}
// Algorítmo Branch and Bound
// Exponencial na runtime
// O uso de memória é O(n), cada chamada recursiva usa O(1) de memória
void branch_and_bound(int n, double (*coords)[2], int *tsp) {
    double (*most)[2] = new double[n][2];
    double (*least)[2] = new double[n][2];
    for(int i = 0; i < n; i += 1)
        most[i][0] = most[i][1] = -INFINITY,
        least[i][0] = least[i][1] = INFINITY;
    for(int i = 0; i < n; i += 1) {
        for(int j = i + 1; j < n; j += 1) {
            double d = hypot(coords[i][0] - coords[j][0], coords[i][1] - coords[j][1]);
            for(int k: {i, j}) {
                if(d > most[k][0]) {
                    most[k][1] = most[k][0];
                    most[k][0] = d;
                } else if(d > most[k][1]) {
                    most[k][1] = d;
                }
                if(d < least[k][0]) {
                    least[k][1] = least[k][0];
                    least[k][0] = d;
                } else if(d < least[k][1]) {
                    least[k][1] = d;
                }
            }
        }
    }
    std::swap(least[0][0], least[0][1]);
    std::swap(most[0][0], most[0][1]);

    double min = 0, max = 0;
    for(int i = 0; i < n; i += 1) {
        min += (least[i][0] + least[i][1]) / 2;
        max += (most[i][0] + most[i][1]) / 2;
    }

    tatt(n, coords, tsp);
    double gmax = calculate_cost(n, coords, tsp);

    bool *vis = new bool[n];
    memset(vis, 0, n * sizeof(bool));
    int *temp_tsp = new int[n];
    temp_tsp[0] = 0;
    vis[0] = 1;
    branch_and_bound_recur(
        1, n, coords, vis, &gmax, most, least, min, max, temp_tsp, tsp
    );

    delete[] vis;
    delete[] most;
    delete[] least;
    delete[] temp_tsp;
}

// Apenas calcula o custo de uma solução do TSP
double calculate_cost(int n, double (*coords)[2], int *tsp) {
    bool *vis = new bool[n];
    memset(vis, 0, n * sizeof(bool));
    for(int i = 0; i < n; i += 1) {
        assert(tsp[i] < n);
        assert(!vis[tsp[i]]);
        vis[tsp[i]] = 1;
    }
    for(int i = 0; i < n; i += 1)
        assert(vis[i]);
    delete[] vis;

    double cost = 0;
    for(int i = 0; i < n; i += 1) {
        int a = tsp[i>0 ? i-1 : n-1], b = tsp[i];
        cost += hypot(coords[a][0] - coords[b][0], coords[a][1] - coords[b][1]);
    }
    return cost;
}

int main(int argc, char **argv) {
    if(argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo> <algoritmo>", argv[0]);
        return 1;
    }
    enum Algo {
        None,
        TATT,
        CF,
        BAB
    };
    enum Algo algo = None;
    if(strcmp(argv[2], "tatt") == 0) algo = TATT;
    if(strcmp(argv[2], "cf") == 0) algo = CF;
    if(strcmp(argv[2], "bab") == 0) algo = BAB;
    if(algo == None) {
        fprintf(stderr, "Algorítmo deve ser um de:\n"
            "  tatt - Twice Around the Tree\n"
            "  cf   - Christofides\n"
            "  bab  - Branch and Bound\n"
            "Found: '%s'\n",
            argv[2]
        );
        return 1;
    }

    int err = 0;
    char *name = NULL, *comment = NULL, key[32];
    FILE *f = fopen(argv[1], "r");
    int n;
    while(1) {
        if(fscanf(f, "%31[^ :\n] %*[:] %*[ ]", key) != 1) {
            fprintf(stderr, "Esperava uma chave\n");
            goto bad_format;
        }
        if(strcmp(key, "NODE_COORD_SECTION") == 0) break;
        if(strcmp(key, "NAME") == 0) {
            if(fscanf(f, "%m[^\n]\n", &name) != 1) {
                fprintf(stderr, "Esperava um nome\n");
                goto bad_format;
            }
        } else if(strcmp(key, "COMMENT") == 0) {
            if(fscanf(f, "%m[^\n]\n", &comment) != 1) {
                fprintf(stderr, "Esperava um comentário\n");
                goto bad_format;
            }
        } else if(strcmp(key, "TYPE") == 0) {
            if(fscanf(f, "TSP\n") != 0) {
                fprintf(stderr, "Tipo deve ser TSP\n");
                goto bad_format;
            }
        } else if(strcmp(key, "DIMENSION") == 0) {
            if(fscanf(f, "%d\n", &n) != 1) {
                fprintf(stderr, "Dimensão deve ser um número\n");
                goto bad_format;
            }
        } else if(strcmp(key, "EDGE_WEIGHT_TYPE") == 0) {
            if(fscanf(f, "EUC_2D\n") != 0) {
                fprintf(stderr, "Espaço deve ser EUC_2D\n");
                goto bad_format;
            }
        }
    }
    printf("name is %s\n", name);
    printf("comment is %s\n", comment);
    {
        double (*coords)[2] = new double[n][2];
        for(int i = 0; i < n; i += 1) {
            int k;
            double x, y;
            if(fscanf(f, "%d %lf %lf\n", &k, &x, &y) != 3) goto bad_format;
            coords[k-1][0] = x;
            coords[k-1][1] = y;
        }
        int *tsp = new int[n];
        switch(algo) {
            case TATT:
                tatt(n, coords, tsp);
                break;
            case CF:
                christofides(n, coords, tsp);
                break;
            case BAB:
                branch_and_bound(n, coords, tsp);
                break;
            case None:
                return 1;
        }
        printf("Total cost was %lf\n", calculate_cost(n, coords, tsp));

        delete[] tsp;
        delete[] coords;
    }
    if(0) bad_format: {
        err = 1;
        fprintf(stderr, "O formato do arquivo não corresponde a um problema TSP da TSPLIB\n");
        return 1;
    }
    fclose(f);
    free(name);
    free(comment);
    return err;
}
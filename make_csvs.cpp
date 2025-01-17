#include <stdio.h>
#include <map>
#include <string>

using namespace std;

int produce_tsplib_csv() {
    FILE *f = fopen("results/results_tsplib.log", "r");
    if(!f) return 1;
    if(fscanf(f, "%*[^\n]\n") != 0) return 1;
    FILE *g = fopen("results/results_tsplib.csv", "w");

    char *file, *algo;
    int time;
    double cost;

    map<string, map<string, int>> times;
    map<string, map<string, double>> costs;
    while(fscanf(f, "%m[^,],%m[^,],%d,%lf\n", &file, &algo, &time, &cost) == 4) {
        string form(file);
        form = form.substr(form.find_last_of("/") + 1);
        times[form][algo] = time;
        costs[form][algo] = cost;
    }
    free(file);
    free(algo);
    fprintf(g, "File,Time TATT,Time CF,Cost TATT,Cost CF\n");
    for(auto &pair: times) {
        auto& file = pair.first;
        auto &times_ = pair.second;
        auto& costs_ = costs[file];
        fprintf(g, "%s,%d,%d,%lf,%lf\n", file.data(), times_["tatt"], times_["cf"], costs_["tatt"], costs_["cf"]);
    }

    fclose(f);
    fclose(g);
    return 0;
}

int produce_random_csv() {
    FILE *f = fopen("results/results_bab.log", "r");
    FILE *g = fopen("results/results_poly.log", "r");
    if(!f || !g) return 1;
    if(fscanf(f, "%*[^\n]\n") != 0) return 1;
    if(fscanf(g, "%*[^\n]\n") != 0) return 1;
    FILE *h = fopen("results/results_random.csv", "w");

    char *algo;
    double cost, costs[128][3] = {};
    int time, times[128][3] = {}, n, lines = 0;
    while(fscanf(f, "%d,%d,%lf\n", &n, &time, &cost) == 3) {
        int algo_num = 0;
        times[n][algo_num] = time;
        costs[n][algo_num] = cost;
        lines = std::max(lines, n+1);
    }
    while(fscanf(g, "%d,%m[^,],%d,%lf\n", &n, &algo, &time, &cost) == 4) {
        int algo_num = 0;
        if(algo == string("tatt")) algo_num = 1;
        else if(algo == string("cf")) algo_num = 2;
        times[n][algo_num] = time;
        costs[n][algo_num] = cost;
    }
    free(algo);
    fprintf(h, "Count,Time BAB,Time TATT,Time CF,Cost BAB,Cost TATT,Cost CF\n");
    for(int i = 3; i < lines; i += 1)
        fprintf(h, "%d,%d,%d,%d,%lf,%lf,%lf\n", i, times[i][0], times[i][1], times[i][2], costs[i][0], costs[i][1], costs[i][2]);

    fclose(f);
    fclose(g);
    fclose(h);
    return 0;
}

int main() {
    produce_tsplib_csv();
    produce_random_csv();
}
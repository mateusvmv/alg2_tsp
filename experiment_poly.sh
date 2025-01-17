#!/usr/bin/env bash
./compile.sh

OUTPUT_DIR="random"
RESULTS_FILE="results/results_poly.log"
MAX_JOBS=2
MAX_TIME=1800

cleanup() {
    echo "Script terminated. Cleaning up..."
    pkill -P $$
    exit 1
}
trap cleanup SIGINT SIGTERM

mkdir -p $OUTPUT_DIR

echo "Number,Algorithm,Time (ms),Cost" > $RESULTS_FILE

process_iteration() {
    local num=$1
    local output_dir=$2
    local results_file=$3
    local max_time=$4
    local algo=$5

    local filename="${output_dir}/${num}.tsp"

    if test -f $filename; then
        echo "File reused: $filename"
    else
        echo "Generating file: $filename"
        ./bin/make_random "$filename" "$num"
    fi

    echo "Solving for: $filename" "$algo"
    local start_time=$(date +%s%3N)
    output=$(timeout "$max_time" ./bin/solve "$filename" "$algo")
    local solve_exit_code=$?
    local end_time=$(date +%s%3N)

    local elapsed_time=$((end_time - start_time))

    if [ $solve_exit_code -eq 124 ]; then
        echo "Solver exceeded the $max_time ms limit for $filename."
        TIMED_OUT=1
        return
    fi

    cost=$(echo "$output" | grep -oP 'Total cost was \K[0-9]+\.[0-9]+')

    echo "$num,$algo,$elapsed_time,$cost" >> "$results_file"
    echo "Run for $filename $algo completed in $elapsed_time ms."
}

current_jobs=0
for NUMBER in {3..30}; do
    for ALGO in tatt cf; do
        if ! ((current_jobs < MAX_JOBS)); then
            wait -n
            ((current_jobs--))
        fi
        process_iteration "$NUMBER" "$OUTPUT_DIR" "$RESULTS_FILE" "$MAX_TIME" "$ALGO" &
        ((current_jobs++));
    done
done

echo "Script finished. Results saved to $RESULTS_FILE."

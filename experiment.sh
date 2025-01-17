#!/bin/bash
./compile.sh

# Define constants
OUTPUT_DIR="random"
RESULTS_FILE="results.log"
MAX_TIME=1800  # 30 minutes in seconds
# MAX_TIME=3600  # 30 minutes in seconds
NUMBER=3       # Starting number
MAX_JOBS=4

cleanup() {
    echo "Script terminated. Cleaning up..."
    pkill -P $$
    exit 1
}

# Trap SIGINT (Ctrl+C), SIGTERM, and other common termination signals
trap cleanup SIGINT SIGTERM

# Ensure the output directory exists
mkdir -p $OUTPUT_DIR

# Clear results file
echo "Number,Time(ms),Cost" > $RESULTS_FILE

# Stop
TIMED_OUT=0

# Function to process a single iteration
process_iteration() {
    local num=$1
    local output_dir=$2
    local results_file=$3
    local max_time=$4

    # Generate the filename
    local filename="${output_dir}/${num}.tsp"

    # Run the ./make_random program
    if test -f $filename; then
        echo "File reused: $filename"
    else
        echo "Generating file: $filename"
        ./bin/make_random "$filename" "$num"
    fi

    # Time the ./solve program
    echo "Solving for: $filename"
    local start_time=$(date +%s%3N)
    output=$(timeout "$max_time" ./bin/solve "$filename" bab)
    local solve_exit_code=$?

    local end_time=$(date +%s%3N)

    # Calculate elapsed time
    local elapsed_time=$((end_time - start_time))

    # Check if the process was terminated by timeout
    if [ $solve_exit_code -eq 124 ]; then
        echo "Solver exceeded the $max_time ms limit for $filename."
        TIMED_OUT=1
        return
    fi

    cost=$(echo "$output" | grep -oP 'Total cost was \K[0-9]+\.[0-9]+')

    # Log the result
    echo "$num,$elapsed_time,$cost" >> "$results_file"
    echo "Run for $filename completed in $elapsed_time ms."
}

# Main loop
current_jobs=0

while [[ $TIMED_OUT == 0 ]]; do
    if ((current_jobs < MAX_JOBS)); then
        # Start a new job
        process_iteration "$NUMBER" "$OUTPUT_DIR" "$RESULTS_FILE" "$MAX_TIME" &
        ((current_jobs++))
        ((NUMBER++))
    else
        # Wait for any job to finish before starting a new one
        wait -n
        ((current_jobs--))
    fi
done

echo "Script finished. Results saved to $RESULTS_FILE."

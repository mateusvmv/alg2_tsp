#!/usr/bin/env bash
./compile.sh

max_time=1800
results_file="results_tsplib.log"
input_folder="tsps"
# max_jobs=$(nproc)
max_jobs=2
semaphore="/tmp/run_algorithms_semaphore"

echo "Filename,Algorithm,ElapsedTime,Cost" > "$results_file"

run_instance() {
    local filename="$1"
    local algo="$2"

    local start_time=$(date +%s%3N)
    local output=$(timeout "$max_time" ./solve "$filename" "$algo")
    local end_time=$(date +%s%3N)

    local elapsed_time=$((end_time - start_time))
    local cost=$(echo "$output" | grep -oP 'Total cost was \K[0-9]+\.[0-9]+')

    echo "$filename,$algo,$elapsed_time,$cost" >> "$results_file"
}

mkfifo "$semaphore" || exit 1
exec 3<>"$semaphore"
rm "$semaphore"

for ((i = 0; i < max_jobs; i++)); do
    echo "" >&3
done

for filepath in $(grep -rl "EDGE_WEIGHT_TYPE : EUC_2D" "$input_folder"); do
    for algo in tatt cf; do
        # Acquire a semaphore token (wait for one to become available)
        read -r <&3

        # Run the instance in a subshell
        {
            run_instance "$filepath" "$algo"

            # Release the semaphore token
            echo "" >&3
        } &
    done
done

wait

# Close the semaphore
exec 3<&-
exec 3>&-

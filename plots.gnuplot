# Set dark theme for the plot
set terminal pngcairo size 800,600 enhanced font 'Arial,10' # background rgb "#121212"

# set grid linecolor rgb "#444444"
# set border linewidth 1.5 linecolor rgb "#555555"
# set key font ",12" textcolor rgb "white"

# Set up basic styles
# set style line 1 lc rgb '#ff7f0e' lt 1
# set style line 2 lc rgb '#1f77b4' lt 1
# set style line 3 lc rgb '#BBAAFF' lt 1

# Set the labels and titles for the time comparison plot
set xlabel "Time (TATT)" # textcolor rgb "white"
set ylabel "Time (ms)" # textcolor rgb "white"
set title "Time Comparison: TATT vs CF" # textcolor rgb "white"

# Set datafile separator and load the data
set datafile separator ","

# Plot Time TATT vs Time CF
set output 'tsplib_plot_times.png'
set autoscale
plot 'results/results_tsplib.csv' using 2:2 title 'Time TATT' smooth unique with linespoints ls 1, \
     '' using 2:3 title 'Time CF' smooth unique with linespoints ls 2

set output 'tsplib_plot_times_zoom.png'
set xrange [0:500]
plot 'results/results_tsplib.csv' using 2:2 title 'Time TATT' smooth unique with linespoints ls 1, \
     '' using 2:3 title 'Time CF' smooth unique with linespoints ls 2

# Now switch to cost comparison plot
# Set labels and title for cost comparison plot
set xlabel "Cost (TATT)"
set ylabel "Cost"
set title "Cost Comparison: TATT vs CF"

set output 'tsplib_plot_costs.png'
set autoscale
plot 'results/results_tsplib.csv' using 4:4 title 'Cost TATT' smooth unique with linespoints ls 1, \
     '' using 4:5 title 'Cost CF' smooth unique with linespoints ls 2

set output 'tsplib_plot_costs_zoom.png'
set xrange [0:3000000]
plot 'results/results_tsplib.csv' using 4:4 title 'Cost TATT' smooth unique with linespoints ls 1, \
     '' using 4:5 title 'Cost CF' smooth unique with linespoints ls 2


set xlabel "# Cities"
set ylabel "Time"
set title "Time Comparison: Random Cases"

set output 'random_plot_times.png'
set autoscale
plot 'results/results_random.csv' using 1:2 title 'Time BAB' with linespoints ls 1, \
     '' using 1:3 title 'Time TATT' with linespoints ls 2, \
     '' using 1:4 title 'Time CF' with linespoints ls 3

set xlabel "# Cities"
set ylabel "Cost"
set title "Cost Comparison: Random Cases"

set output 'random_plot_costs.png'
set autoscale
plot 'results/results_random.csv' using 1:5 title 'Cost BAB' with linespoints ls 1, \
     '' using 1:6 title 'Cost TATT' with linespoints ls 2, \
     '' using 1:7 title 'Cost CF' with linespoints ls 3
datafile = "results/with_optima.csv"
set datafile separator ","

set terminal pngcairo size 800,600
set output "factors.png"

set title "Approximation Factors for each Case"
set xlabel "Cases"
set ylabel "Factors"

set grid
set key outside

unset xtics

plot datafile using ($4/$6):xtic(1) with points ls 1 title "TATT Factor", \
     datafile using ($5/$6):xtic(1) with points ls 2 title "CF Factor"

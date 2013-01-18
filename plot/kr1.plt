set autoscale

set grid

set xdata time

set xlabel "czas [h:m:s]"
set ylabel "Masa [kg]"

set timefmt "%H:%M:%S"
#set xr [0.0:4:400.0]

#set format x "%H:%M:%S"

set title "KR"
plot "kr.txt" using 1:3 title 'Masa' with lines


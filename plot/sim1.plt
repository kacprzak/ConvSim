set autoscale
set macro

PLOT = "plot filename using 1:($2*3600/4) title 'Węglanowa' with lines, \
             filename using 1:($3*3600/4) title 'Piaskowcowa' with lines, \
             filename using 1:($4*3600/4) title 'Łupkowa' with lines, \
             filename using 1:(($2+$3+$4)*3600/4) title 'Suma' with lines"
    
set multiplot;                          # get into multiplot mode
set size 1,0.5;  
set origin 0.0,0.5;

set xlabel "czas [s]"
set ylabel "Q [t/h]"

filename = "p3.dat"
@PLOT

set origin 0.0,0.0;

set xlabel "czas [s]"
set ylabel "Q [t/h]"

filename = "p5.dat"
@PLOT

unset multiplot                         # exit multiplot mode

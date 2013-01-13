set autoscale
set macro

PLOT = "plot filename using 1:($2/($2+$3+$4)) title 'Węglanowa' with lines, \
             filename using 1:($3/($2+$3+$4)) title 'Piaskowcowa' with lines, \
             filename using 1:($4/($2+$3+$4)) title 'Łupkowa' with lines"
#             filename using 1:($2+$3+$4)*3600/4) title 'Suma' with lines"
    
set xlabel "czas [s]"
set ylabel "Udział rudy [%]"

set xr [0:]
set yr [0.0:1.0]

set multiplot
set size 1,0.2
  
set origin 0.0,0.8
set title "P-1"
filename = "p1.dat"
@PLOT

set origin 0.0,0.6
set title "P-2"
filename = "p2.dat"
@PLOT

set origin 0.0,0.4
set title "P-3"
filename = "p3.dat"
@PLOT

set origin 0.0,0.2
set title "P-4"
filename = "p4.dat"
@PLOT

set origin 0.0,0.0
set title "P-5"
filename = "p5.dat"
@PLOT

unset multiplot

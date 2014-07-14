set title "Statistical analysis of the temperature history (box plot)"

#set terminal png size 1700,860 enhanced font "Verdana,10"
#set terminal pngcairo size 1700,860 enhanced font 'Verdana,10'
#set output filename.'.png'
set terminal svg size 1700,860 fname 'Verdana' fsize 10
set output filename.'.svg'

set xdata time
#---- days
set timefmt "%Y-%m-%d"
set xtics format "%b %d"
#---- weeks
#set xtics format "week %W (%b)"
#---- months
#set timefmt "%Y-%m"
#set xtics "2012-01",2592000,"2018-12"
#set xrange ["2014-01":"2014-12"]
#unset mxtics
#set xtics format "%b"
#----
set xlabel "Date" offset 0
set ylabel "Temperature °C" offset 0

set grid y back
set ytics axis in



#set style fill transparent solid 0.8 noborder

plot filename using 1:3:2:6:5:($8 * 250) with candlesticks lt 3 lw 2 title 'Quartiles' whiskerbars, \
     ''       using 1:4:4:4:4:($8 * 250) with candlesticks lt -1 lw 2 notitle, \
     ''       using 1:7 with p ls 1 title 'Mean Temperature'
#    ''       using 1:7 with linespoints lt 3 pt 3 notitle, \
#    ''       using 1:7 smooth bezier with lines lt rgb "violet" lw 2 title 'Mittlere Temperatur (geglättet)'
#    ''       using 1:4 smooth bezier with lines, \

set datafile separator ';'

set title "Masse mesurée en fonction du rapport cyclique"
set xlabel "Rapport cyclique du signal PWM envoyé à l'ESC"
set ylabel "masse mesurée (en gramme)"

unset key

set terminal png size 2048,1280 

set output "output.png"
plot "file.csv" using 1:3 with line

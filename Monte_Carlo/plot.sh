#!/usr/bin/bash

make clean && make monte_carlo

#First grapth
./monte_carlo -n 1000  | tail -n +2 | awk {'print $3, $4, $5'} > one_checker.dat #tail -n +2 removes the first row

awk '{if ($3 == 0) print $1, $2}' one_checker.dat > red_dots.dat #checking if the value in the third column is equal to 0. if it is, add it to the red_dots.dat file
awk '{if ($3 == 1) print $1, $2}' one_checker.dat > blue_dots.dat #checking if the value in the third column is equal to 0. if it is, add it to the blue_dots.dat file



gnuplot << EOF
set terminal pdf #sets output format as pdf
set output "plot.pdf" #output pdf will be called plot.pdf
set size square 1,1 #makes the graph in the shape of a square
#set key autotitle columnhead
set key off #removes the key on the top right of the graph
set xrange[0:1] #sets the x axis range
set yrange[0:1] #sets the y axis range
set xlabel "x"
set ylabel "y"
plot "red_dots.dat" with dots lc rgb "red", "blue_dots.dat" with dots lc rgb "blue", [x=0:1] sqrt(1-x**2) lc rgb "black" #plots the respective cooridinates with the respective color. Also graphing the half circle
EOF

#removing all dat files used for organization
rm red_dots.dat
rm blue_dots.dat
rm one_checker.dat


#Second graph
pi=3.14159 #initializing value of pi

#doing this 4 times. saving the first columns from monte carlo to a dat file. Then comparing the second column value to the value of pu and storing the value in another dat file. Store the first column from the previous dat file in the new one
#after each iteration, sleep for one second to be able to have a differnt seed
./monte_carlo -n 20000 | tail -n +2 | awk {'print $1, $2'} > estamate_one.dat
awk '{print $1, '$pi'-$2}' estamate_one.dat > difference1.dat
sleep 1

./monte_carlo -n 20000 | tail -n +2 | awk {'print $1, $2'} > estamate_two.dat
awk '{print $1, '$pi'-$2}' estamate_two.dat > difference2.dat
sleep 1

./monte_carlo -n 20000 | tail -n +2 | awk {'print $1, $2'} > estamate_three.dat
awk '{print $1, '$pi'-$2}' estamate_three.dat > difference3.dat

./monte_carlo -n 20000 | tail -n +2 | awk {'print $1, $2'} > estamate_four.dat
awk '{print $1, '$pi'-$2}' estamate_four.dat > difference4.dat
sleep 1

gnuplot << EOF
set terminal pdf
set output "Monte_Carlo_Error_Estimation.pdf"
set title "\Monte Carlo Error Estimation" #title of the graph
set xlabel "# of iterations" #label for x axis
set ylabel "Error" #labeling the y axis
set key off
set xrange [1:20000]
set zeroaxis
set logscale x 2
set xtics add (1, 4, 16, 64, 256, 1024, 4096, 16384) #custom x ticks
set yrange [-1:1]
plot "difference1.dat" with lines, "difference2.dat" with lines, "difference3.dat" with lines, "difference4.dat" with lines

EOF

#removing all dat files used for organization

rm estamate_one.dat
rm difference1.dat
rm estamate_two.dat
rm difference2.dat
rm estamate_three.dat
rm difference3.dat
rm estamate_four.dat
rm difference4.dat


#!/bin/bash
echo -n "" > latency.csv
echo -e "RunNumber\ttime_namelookup\ttime_connect\ttime_appconnect\ttime_pretransfer\ttime_redirect\ttime_starttransfer\ttime_total" | tee -a latency.csv

#gnupot 

echo "set term postscript eps enhanced" > GNUPLOT
echo "set output 'lat.eps'" >> GNUPLOT
echo "set xlabel 'Requests'" >> GNUPLOT
echo "set ylabel 'Times'" >> GNUPLOT
echo "set title 'cpu and memory usage overtime'" >> GNUPLOT
echo "plot 'latency.csv' using 1:2 title 'time_namelookup' with lines lt 6 lw 3 lc 1 , 'latency.csv' using 1:3 title 'time_connect' with lines lt 5 lw 3 lc rgb 1 , 'latency.csv' using 1:4 title 'time_appconnect' with lines lt 4 lw 3 lc 3 , 'latency.csv' using 1:5 title 'pretransfer' with lines lt 3 lw 3 lc 4 , 'latency.csv' using 1:7 title 'start_transfer' with lines lt 2 lw 3 lc 5 , 'latency.csv' using 1:8 title 'total_time' with lines lt 1 lw 3 lc 6" >> GNUPLOT
#gnuplot part end




declare -i req
req=0
while true
do
    ((req++))
    res=$(curl -k -w "@curl-format.txt" -o /dev/null -s "https://localhost:8443/hello")
    echo -e "$req\t$res" | tee -a latency.csv
    sleep 0.5
done

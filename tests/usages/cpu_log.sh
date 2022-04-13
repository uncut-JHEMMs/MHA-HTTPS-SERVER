#!/bin/bash
echo -n "" > cpu_usage_log.csv
PNAME="$1"
PID=$(pidof ${PNAME})
echo -e "datetime\tCPU\tMEM" | tee -a cpu_usage_log.csv

#gnu plot config start

echo "set term postscript eps enhanced" > GNUPLOT
echo "set output 'cpuusage.eps'" >> GNUPLOT
echo "set xlabel 'Time'" >> GNUPLOT
echo "set ylabel 'CPUU/S and MEMu/S'" >> GNUPLOT
echo "set title 'cpu and memory usage overtime'" >> GNUPLOT
echo "plot 'cpu_usage_log.csv' using 1:2 title 'CPU_USAGE' with lines lt 1 lw 3, 'cpu_usage_log.csv' using 1:3 title 'MEMORY_USAGE' with lines lt 2 lw 1" >> GNUPLOT

#gnu plot config end

while true
do
    cusage=$(top -p $PID -n1 | awk '(NR > 7) {print $10}')
    musage=$(top -p $PID -n1 | awk '(NR > 7) {print $11}')
    sleep 0.3
    dat=$(date)
    echo -e "$dat\t$cusage\t$musage" | tee -a cpu_usage_log.csv
done 

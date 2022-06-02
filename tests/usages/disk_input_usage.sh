#!/bin/bash
echo -n "" > disk_usage_log.csv
PNAME="$1"
PID=$(pidof ${PNAME})
#gnu plot config start

echo "set term postscript eps enhanced" > GNUPLOT
echo "set output 'diskusage.eps'" >> GNUPLOT
echo "set xlabel 'datetime'" >> GNUPLOT
echo "set ylabel 'Bytes/S'" >> GNUPLOT
echo "set title 'diskusage over time'" >> GNUPLOT
echo "plot 'disk_usage_log.csv' using 1:2 title 'input_usage' with lines lt 1 lw 3, 'disk_usage_log.csv' using 1:3 title 'write_usage' with lines lt 2 lw 1" >> GNUPLOT

#gnu plot config end


echo -e "datetime\tdiskread\tdiskwrite" | tee -a disk_usage_log.csv
while true
do
    read=$(iotop -b -p $PID -n1 | awk '(NR > 3) {print $4}') 
    write=$(iotop -b -p $PID -n1 | awk '(NR > 3) {print $6}')
    dat=$(date)
    echo -e "$dat\t$read\t$write" | tee -a disk_usage_log.csv
    sleep 0.1
done

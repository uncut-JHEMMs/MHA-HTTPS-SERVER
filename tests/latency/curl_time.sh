#!/bin/bash
echo -n "" > latency.csv
echo -e "time_namelookup\ttime_connect\ttime_appconnect\ttime_pretransfer\ttime_redirect\ttime_starttransfer\ttime_total" | tee -a latency.csv
while true
do
    curl -k -w "@curl-format.txt" -o /dev/null -s "https://localhost:8443/hello" >> latency.csv
    sleep 0.5
done

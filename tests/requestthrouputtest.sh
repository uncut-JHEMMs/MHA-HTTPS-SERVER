#!/bin/bash
echo -n "" > spamapi.csv
path=$1
declare -i max
max=0
declare -i passed
declare -i failed
declare -i total_reqs
total_reqs=0
passed=0
failed=0
echo -e "status\treqs/s" | tee -a spamapi.csv
get()
{
    
    status=$(curl -o dev/null -k -s -w "%{http_code}\n" https://localhost:8443/$path)  
    ((total_reqs++))
    #statue check add passsed req or fialed req
    if ! [ ${status} -eq 200 ];then
        echo "iside if"
        ((failed++))
    else
        ((passed++))
    fi
    
    echo -e "$status\t$max" | tee -a spamapi.csv 
}


while true
do
    max=$max+20
    #echo "rate $max / s"
    for i in `seq 1 $max`
    do
        get &
    done
    sleep 1
done
echo -e "passed = $passed\tfailed = $failed\t total_reqs = $total_reqs" | tee -a spamapi.csv

#get cpud usage top -p $(pidof net.out) -b -d1 -n1 | grep -i "%Cpu(s)" |head -c21 |cut -d ' ' -f3

#top -b -d 2 -p (pidof net.out) | awk \ -v cpuLog="" -v pid="$PID" -v pname="$PNAME" '


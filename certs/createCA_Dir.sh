#!/bin/bash

ca=$1
mkdir $ca $ca/ca                                                        


cd $ca/ca/

mkdir crl pfx private certs newcerts

echo "01" > serial
echo 1000 > crlnumber
touch index.txt

chmod 700 private

#!/bin/bash

clear

if [ $1 == test ] || [ $1 == Test ]
then
    #test if certificate was generated
    echo "Verify root CA certificate"
    openssl x509 -noout -text -in root/ca/certs/root.ca.cert.pem

    echo
    echo "Verify server certificate was signed by root"
    signingcert=root/ca/certs/root.ca.cert.pem
    signedcert=server/ca/certs/server.ca.cert.pem
    openssl verify -CAfile $signingcert $signedcert

    echo
    echo "verify document certificate was signed by root"
    signedcert=document/ca/certs/document.ca.cert.pem
    openssl verify -CAfile $signingcert $signedcert

    echo
    echo "Verify root crl"
    openssl crl -in root/ca/crl/root.ca.crl -noout -text

    echo
    echo "Verify server crl"
    openssl crl -in server/ca/crl/server.ca.crl -noout -text 

    echo
    echo "Verify document crl"
    openssl crl -in document/ca/crl/document.ca.crl -noout -text 

    echo
    echo "Verify server pfx bundle"
    openssl pkcs12 -info -in server/ca/pfx/server.ca.pfx

    echo
    echo "Verify document pfx bundle"
    openssl pkcs12 -info -in document/ca/pfx/document.ca.pfx
    
    #Generate CA.
else

    #Generate directories
    ./createCA_Dir.sh root
    
    ./createCA_Dir.sh server

    ./createCA_Dir.sh document

    mv openssl-sca.cnf server/ca/
    mv openssl-dca.cnf document/ca/
    
    #Generate a private key for root
    key=root/ca/private/root.ca.key.pem
    cert=root/ca/certs/root.ca.cert.pem
    
    openssl genrsa -out $key 4096
    chmod 400 $key
    
    #Generate the certificate authority for root.
    openssl req -new -x509 -config openssl-rca.cnf -days 365000 -key $key -out $cert
    chmod 444 $cert

    #Generate CRL for root
    openssl ca -config openssl-rca.cnf -gencrl -out root/ca/crl/root.ca.crl
    
    
    #Generate server CA

    #Generate a private key for server
    
    key=server/ca/private/server.ca.key.pem
    cert=server/ca/certs/server.ca.cert.pem
    csr=server/ca/server.csr

    ./createCSR.sh $key $csr
    
    #Generate the server ca from csr
    openssl ca -config openssl-rca.cnf -out $cert -in $csr

    #Generate server CRL
    openssl ca -config openssl-rca.cnf -gencrl -out server/ca/crl/server.ca.crl

    #Generate PFX bundle for server
    pfx=server/ca/pfx/server.ca.pfx
    
    #append root ca to server ca
    cat root/ca/certs/root.ca.cert.pem >> $cert
    
    openssl pkcs12 -export -out $pfx -inkey $key -in server/ca/certs/server.ca.cert.pem
    chmod 444 $cert
    

    #Generate csr for document
    key=document/ca/private/document.ca.key.pem
    cert=document/ca/certs/document.ca.cert.pem
    csr=document/ca/document.csr
    
    #Generate the csr for document.
    ./createCSR.sh $key $csr
    
    #sign document csr
    openssl ca -config openssl-rca.cnf -out $cert -in $csr    

    #Generate CRL for document
    openssl ca -config openssl-rca.cnf -gencrl -out document/ca/crl/document.ca.crl

    #Generate PFX bundle for document
    
    pfx=document/ca/pfx/document.ca.pfx
    
    cat root/ca/certs/root.ca.cert.pem >> $cert
    chmod 444 $cert

    openssl pkcs12 -export -out $pfx -inkey $key -in document/ca/certs/document.ca.cert.pem
    
fi

if [ $1 == test ]
then
    cat $2

else
    
    key=$1
    csr=$2
    
    #Generate key
    openssl genrsa -out $key 4096
    
    chmod 400 $key
    
    #Generate the csr for server.
    openssl req -new -key $key -out $csr

fi


#generate ca.key
openssl genrsa -out ca.key 2048
#generating .crtificate for CA
openssl req -new -x509 -key ca.key -out ca.crt
#generating Public key for certificate authority
openssl x509 -pubkey -in ca.crt > ca-pub.pem

#Signing digest by called dum with ca.key
openssl dgst -sha256 -sign ../ca.key -out ./signed dum
#generating base54 ouput of above line
openssl base64 -in ./signed -out signedB64
#converting back from bas64 
openssl base64 -d -in ./signedB64 -out ./verSigned
#verifying for if the msg is authentic or not
openssl dgst -sha256 -verify ../ca-pub.pem -signature ./verSigned dum

gprof2dot --format=callgrind --output=outfilename.dot filename
dot -Tpng outfilename.dot -o graph.png


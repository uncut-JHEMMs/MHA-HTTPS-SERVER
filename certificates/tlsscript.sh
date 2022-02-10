mkdir -p ca/tls-ca/private ca/tls-ca/db crl certs
chmod 700 ca/tls-ca/private

cp /dev/null ca/tls-ca/db/tls-ca.db
cp /dev/null ca/tls-ca/db/tls-ca.db.attr
echo 01 > ca/tls-ca/db/tls-ca.crt.srl
echo 01 > ca/tls-ca/db/tls-ca.crl.srl

openssl req -new -config tlsconf.cnf -out ca/tls-ca.csr -keyout ca/tls-ca/private/tls-ca.key


openssl ca -config rootconf.cnf -in ca/tls-ca.csr -out ca/tls-ca.crt -extensions signing_ca_ext


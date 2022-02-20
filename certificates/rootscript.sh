#bin/bash!

mkdir -p ca/root-ca/private ca/root-ca/db crl certs
chmod 700 ca/root-ca/private

cp /dev/null ca/root-ca/db/root-ca.db
cp /dev/null ca/root-ca/db/root-ca.db.attr

echo 01 > ca/root-ca/db/root-ca.crt.srl
echo 01 > ca/root-ca/db/root-ca.crl.srl


openssl req -new -config rootconf.cnf -out ca/root-ca.csr -keyout ca/root-ca/private/root-ca.key
openssl ca -selfsign -config rootconf.cnf -in ca/root-ca.csr -out ca/root-ca.crt -extensions root_ca_ext -enddate 20301231235959Z
openssl ca -gencrl -config rootconf.cnf -out crl/root-ca.crl


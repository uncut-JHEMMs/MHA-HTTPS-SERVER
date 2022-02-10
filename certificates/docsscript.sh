mkdir -p ca/docs-ca/private ca/docs-ca/db crl certs
chmod 700 ca/docs-ca/private

cp /dev/null ca/docs-ca/db/docs-ca.db
cp /dev/null ca/docs-ca/db/docs-ca.db.attr
echo 01 > ca/docs-ca/db/docs-ca.crt.srl
echo 01 > ca/docs-ca/db/docs-ca.crl.srl


openssl req -new -config docsconf.cnf -out ca/docs-ca.csr -keyout ca/docs-ca/private/docs-ca.key

openssl ca -config rootconf.cnf -in ca/docs-ca.csr -out ca/docs-ca.crt -extensions software_ca_ext



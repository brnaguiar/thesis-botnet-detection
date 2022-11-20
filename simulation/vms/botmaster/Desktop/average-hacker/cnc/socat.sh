openssl req -newkey rsa:2048 -nodes -keyout cert.key -x509 -days 1000 -out cert.crt
cat cert.key cert.crt > sslfile.pem
socat -d -d -d - ssl-l:65321,reuseaddr,cert=sslfile.pem,verify=0,ignoreeof < netcat_cmds2

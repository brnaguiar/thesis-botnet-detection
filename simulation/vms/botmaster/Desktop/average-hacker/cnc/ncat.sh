#ncat -l -p 65321 --ssl --ssl-cert cert.crt --ssl-key cert.key  < netcat_cmds2.txt #nc -l -p 65321
cat netcat_cmds2.txt - | ncat -l -p 65321 --ssl --ssl-cert cert.crt --ssl-key cert.key 
#ncat -l -p 65321 < netcat_cmds2.sh --ssl --ssl-cert cert.crt --ssl-key cert.key #nc -l -p 65321

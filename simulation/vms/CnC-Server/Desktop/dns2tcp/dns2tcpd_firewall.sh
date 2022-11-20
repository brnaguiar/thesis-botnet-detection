#sudo iptables -t nat -A POSTROUTING -o enp0s3 -j MASQUERADE;\
#sudo iptables -A FORWARD -i enp0s3 -o enp0s3 -m state --state RELATED,ESTABLISHED -j ACCEPT;\
#sudo iptables -A FORWARD -i enp0s3 -o enp0s3 -j ACCEPT;\
sudo iptables -A INPUT -p udp -m udp --dport 53 -j ACCEPT;\
sudo iptables -A OUTPUT -p udp -m udp --sport 53 -j ACCEPT;\

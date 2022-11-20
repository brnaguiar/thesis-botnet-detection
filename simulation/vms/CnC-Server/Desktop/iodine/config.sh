echo labcom | sudo -S sleep 1;\
sudo su -c "echo 1 > /proc/sys/net/ipv4/ip_forward; exit;";\

sudo iptables -t nat -A POSTROUTING -o enp0s3 -j MASQUERADE;\
sudo iptables -A FORWARD -i enp0s3 -o dns0 -m state --state RELATED,ESTABLISHED -j ACCEPT;\
sudo iptables -A FORWARD -i dns0 -o enp0s3 -j ACCEPT;\
sudo iodined -cP labcom  10.0.0.1 t.b0t.pt;\
#sudo ip route add 192.168.10.10/32 via 10.0.0.2 dev dns0-f -D

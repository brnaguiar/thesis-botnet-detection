 #\sudo su && sleep 1
sudo tunctl -t tap0
sudo ifconfig tap0 192.168.2.1 netmask 255.255.255.0 up
sudo iptables -t nat -A POSTROUTING -o  wlp0s20f3 -j MASQUERADE
sudo iptables -A FORWARD -i tap0 -j ACCEPT
# crontap -e --> persists....
sudo echo 1 > /proc/sys/net/ipv4/ip_forward
sudo ip route add 111.11.11.0/24 via 192.168.2.1 dev tap0 
sudo ip route add 192.168.10.0/24 via 192.168.2.1 dev tap0
sudo ip route add 112.12.12.0/24 via 192.168.2.1 dev tap0
sudo ip route add 100.10.10.0/24 via 192.168.2.1 dev tap0        

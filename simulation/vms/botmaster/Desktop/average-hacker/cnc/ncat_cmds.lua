local commands = {
    "echo ola",
    "ls",
    "echo labcom | sudo -S arp",
    "echo labcom | sudo -S arp -a",
    "users",
    "netstat -i",
    "netstat -M",
    "uname -a",
    "groups",
    "tcpdump -n > output",
    "echo labcom | sudo -S insmod rootkit.ko",
    "modprobe -r rootkit.ko",
    "useradd –g 500 –u 500 –s /usr/local/bin/nocando –d /var/spool/vmail",
    "mkdir teste",
    "rm -r teste/",
    "kill -9 12313123 ",
    "wget https://raw.githubusercontent.com/brnaguiar/linreg-c/main/linreg.py ",
    "chattr -i /etc/ld.so.preload",
    "echo labcom | sudo -S cat /etc/shadow > file",
    "echo labcom | sudo -S cat /etc/passwd > file",
    "echo  > ~/.bash_history ",
    "ip a ",
    "ip route ",
    "ls ",
    "cd . ",
    "cd ../cnc ",
    "cd ../../average-hacker/cnc/",
    "cd ../../../dns2tcp/average-hacker/cnc",
    "cd ../../../../Desktop/dns2tcp/average-hacker/cnc",
    "cd ../../../../../../../../home/labcom/Desktop/dns2tcp/average-hacker/cnc",
    "ls -l ",
    "ls -la ",
    "ls ../ ",
    "ls ../../ ",
    "ls ../../../ ",
    "ls ../../../../ ",
    "ls ../low/ ",
    "ls ../../average-hacker/ ",
    "ls ../../../../DNSExfil/ ",
    "ls -l ../../average-hacker/ ",
    "ls -l ../../../../DNSExfil/ ",
    "ls ../../../../.vscode-server/ ",
    "ls -l ../../../../.vscode-server/ ",
    "cat ../../../../.zshrc ",
    "cat ~/.zshrc",
    "cat ~/.xsession-errors, ",
    "cat ~/../../../../.xsession-errors ",
    "touch teste ",
    "rm teste ",
    "ps aux | grep client ",
    "chmod -x teste ",
    "echo \"asdasdasdad\" ",
    "echo labcom | sudo -S ifconfig lo ",
    "ip a show lo",
    "netstat -s",
    "netstat -r",
    "echo \"asdasdasdasdasdasd asd asd asd asds dasdad\" ",
    "echo \"asdasdasdasdasdasdkasdas f fds f dsfsd fsdfsdfsdf sdfs dfsd fs fsd fs fds fsd fsd f sf ssdf sd fsd fsdf sdf \"",
    "echo \"asdkasdasmdsad asd as das das das d asd asd as das d sa das\"",
    "dig google.com ",
    "dig google.com MX ",
    "nslookup mindmajix.com",
    "echo  labcom | sudo -S route ",
    "iwconfig ",
    "curl -O google.com/doodles/childrens-day-2014-multiple-countries ",
    "whois fcporto.pt ",
    "whois ua.pt",
    "pwd",
}

function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

for i=10, 0, -1
do
    math.randomseed(os.time())
    local handle = io.popen(commands[math.random(#commands)] .. " 2>&1")
    local result = handle:read("*a")
    print(result)
    handle:close()
    sleep(2)
end

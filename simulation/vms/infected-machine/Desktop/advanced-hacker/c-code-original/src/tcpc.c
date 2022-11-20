#include <math.h>
#include <stdio.h>
#include <stdlib.h> //
//#include <time.h>
#include <string.h>
#include <unistd.h>  //<><
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "stats/maths_and_stats.h"


void gen_data2send(char ***data_array, int *eof_flag, FILE *file_ptr, int *size_pps, int number_of_pps, int *real_npps)
{
        int i;
        for (i = 0; i < number_of_pps && *eof_flag; i++) 
        {
            //generate pps with exponential distribution 
            int pps = gen_exponential_numbers(0.08);  //gen_exponential_numbers(0.08)
            
            // alloc data_array 2nd dim   
            data_array[i] = (char **) malloc(pps*sizeof(*data_array[i])); // (char *)c

            int j;
            for (j = 0; j < pps && *eof_flag; j++)
            {
                // generage pkt len with normal distribution
                int bytes_to_read = gen_normal_numbers(); //40

                // create buffer to copy contents...  
                char buffer[bytes_to_read+1];

                // read "bytes_to_read" bytes from file
                size_t size_read = fread(buffer, sizeof(char), bytes_to_read, file_ptr);

                // add \0
                buffer[size_read] = '\0';
                
                // alloc 3rd dim of data_array
                data_array[i][j] = (char *) malloc((3+size_read+1)*sizeof(char));

                // add message length to data_array
                sprintf(data_array[i][j], "%03d", size_read);

                // copy from buffer to data_array...
                strcat(data_array[i][j], buffer);

                //printf("\nSIZE BYTES TO READ: %d\nSIZE READ: %ld\nSTR LEN: %ld\n", bytes_to_read, size_read, strlen(data_array[i][j]));
                //printf("STRING: %s\n", data_array[i][j]);

                if (size_read < bytes_to_read)
                    *eof_flag = 0; // end of file...
            }
            size_pps[i] = j;
            // if last (EOF) block size is lesser than the a priori generated block size, dealloc unnecessary memory
            if (pps > j)
                data_array[i] = realloc(data_array[i], (j)*sizeof(*data_array[i])); // dont know if this is redundant...
        }
        *real_npps = i;
        // if last (EOF) block size is lesser than the a priori generated block size, dealloc unnecessary memory
        if (number_of_pps > i)
            data_array = realloc(data_array, (i)*sizeof(**data_array)); // dont know if this is redundant...
}

void free_data(char ***data_array, int *size_pps, int real_npps)
{
        for (int i = 0; i < real_npps; i++)
        {
            for (int j = 0; j < size_pps[i]; j++)
                free(data_array[i][j]);
            free(data_array[i]);
        }
        free(data_array);
        free(size_pps);

}

int main()
{
    // open file to file_pointer .txt // \\ ;; //
    FILE *file_ptr = fopen("../../../files/textfile300k", "rb");  //iii
    int eof_flag = 1;

    // sockets inits
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2048]; // init server_message here

    // create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
    {
        printf("Unable to create socket\n");
        return -1;
    } printf("Socket created successfully");

    // deactivate Nagle's algorithm...
    int flag = 1;
    setsockopt(socket_desc, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(flag));

    // open socket file descriptor
    FILE *socket_file = fdopen(socket_desc, "w+"); 

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4440);  //`200
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //:w111            111

    //set port and IP for the client:
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.2"); //--INADDR_ANY;ß
    client_addr.sin_port = htons(44410);

    if (bind(socket_desc, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
    {
        printf("Can't bind!i\n");
        return -1;
    }

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Unable to connect\n");
        return -1;
    } printf("Connected with server successfully");

    while(eof_flag)
    {
        // generate number of pps to send
        int N_PPS = 7;
        int number_of_pps = ((int) round(random_number(N_PPS))) + 1;
        int real_npps;

        // clear servers buffer 
        memset(server_message, '\0', sizeof(server_message));
        
        // create pps_size helper array to free data_array later...
        int *size_pps = malloc(number_of_pps * sizeof(int));
        
        //init data array 
        char ***data_array = (char ***) malloc(number_of_pps*sizeof(**data_array)); // (char**);   
        
        // create/generate data send
        gen_data2send(data_array, &eof_flag, file_ptr, size_pps, number_of_pps, &real_npps);
       
        printf("\npackets sent: %d", real_npps);
        // FOR LOOP: SEND DATA
        int i, j;
        for (i = 0; i < real_npps; i++) 
        {
            for (j = 0; j < size_pps[i]; j++) 
            {
                int str_len = strlen(data_array[i][j]);
                if ( str_len > 0)
                {
                    // overwrite default TCP buffer size 
                    setsockopt(socket_desc, SOL_SOCKET, SO_SNDBUF, (int *)&str_len, sizeof(str_len));
//                    setsockopt(socket_desc, SOL_SOCKET, SO_RCVBUF, (int *)&str_len, sizeof(str_len));

                    // printf for debug purposes...
                    //printf("\nLEN: %d | FD : %d | MESSAGE: %s\n", str_len, socket_desc, data_array[i][j]);  //√√±±

                    // write to socket file descriptor 
                    write(socket_desc, data_array[i][j], str_len);

                    // flush data in socket file descriptor 
                    fflush(socket_file);

                }
            }
            int small_sleep = random_number(6)+1;
            printf("\n SMALL_SLEEP: %d", small_sleep);
            sleep(small_sleep);////
        }
        int big_sleep = random_number(60)+30;
        printf("\n BIG _ SLEEP: %d\n", big_sleep);
        sleep(big_sleep);

        // dealloc/free dynamic(heap) data
        free_data(data_array, size_pps, real_npps);
    }
    close(socket_desc);
    fclose(file_ptr);
    return 0;
}


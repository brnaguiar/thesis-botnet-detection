// this version uses python distrubutions.py instead of maths_and_stats.h distributions

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "../stats/maths_and_stats.h"
#include <Python.h>

int load_dists(PyObject *pModule, char *filename_pps, char *filename_pktlen, char *filename_silencee)
{
    PyObject *pFunction, *pythonArgs, *pValue; 
    PyObject *pFileNamePPS, *pFileNamePKTLEN, *pFileNameSilence; 

    if (pModule != NULL) 
    {
        pFunction = PyObject_GetAttrString(pModule, "load_dist_weights_plus_silence");
        
        if (pFunction && PyCallable_Check(pFunction))
        {
            pythonArgs = PyTuple_New(3);
            pFileNamePPS = PyUnicode_FromString(filename_pps);
            pFileNamePKTLEN = PyUnicode_FromString(filename_pktlen);
            pFileNameSilence = PyUnicode_FromString(filename_silencee);
            if (!pFileNamePPS || !pFileNamePKTLEN || !pFileNameSilence)
            {
                Py_DECREF(pythonArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return -1;
            }
            PyTuple_SetItem(pythonArgs, 0, pFileNamePPS);
            PyTuple_SetItem(pythonArgs, 1, pFileNamePKTLEN);
            PyTuple_SetItem(pythonArgs, 2, pFileNameSilence);
        
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
            fprintf(stderr, "Cannot find function \"%s\"\n", "load_dist_weights");
        }
        Py_XDECREF(pFunction);
        Py_DECREF(pModule);
    } else 
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "dis2.py");
        return -1;
    }
}
int empirical_prob(PyObject *pModule, char *pps_or_pktlen)
{
    PyObject *pFunction, *pythonArgs, *pValue; //; //*pDict, ; 
    PyObject *pPPS_OR_PKTLEN; //v/d/*pArgs, *pValue;

    if (pModule != NULL) 
    {
        pFunction = PyObject_GetAttrString(pModule, "empirical_prob");
        
        if (pFunction && PyCallable_Check(pFunction))
        {
            pythonArgs = PyTuple_New(1);
            pPPS_OR_PKTLEN = PyUnicode_FromString(pps_or_pktlen);
            if (!pPPS_OR_PKTLEN)
            {
                Py_DECREF(pythonArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return -1;
            }
            PyTuple_SetItem(pythonArgs, 0, pPPS_OR_PKTLEN);
        
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
            fprintf(stderr, "Cannot find function \"%s\"\n", "empirical_prob");
        }
        Py_XDECREF(pFunction);
        Py_DECREF(pModule);
    } else 
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "dis2.py");
        return -1;
    }
}

int gen_data2send(PyObject *pModule, char **data_array, int *eof_flag, FILE *file_ptr, int pps)
{
   
    int j;
    for (j = 0; j < pps && *eof_flag; j++)
    {
        int bytes_to_read = empirical_prob(pModule, "pktlen");

        // create buffer to copy contents...  
        char buffer[bytes_to_read+1];

        // read "bytes_to_read" bytes from file
        size_t size_read = fread(buffer, sizeof(char), bytes_to_read, file_ptr);

        // add \0
        buffer[size_read] = '\0';
        
        // alloc 3rd dim of data_array
        data_array[j] = (char *) malloc((3+size_read+1)*sizeof(char));

        // add message length to data_array
        sprintf(data_array[j], "%03d", size_read);

        // copy from buffer to data_array...
        strcat(data_array[j], buffer);

        //printf("\nSIZE BYTES TO READ: %d\nSIZE READ: %ld\nSTR LEN: %ld\n", bytes_to_read, size_read, strlen(data_array[i][j]));
        //printf("STRING: %s\n", data_array[i][j]);

        if (size_read < bytes_to_read)
            *eof_flag = 0; // end of file...
    }
    //
    // if last (EOF) block size is lesser than the a priori generated block size, dealloc unnecessary memory
    if (pps > j)
        data_array = realloc(data_array, (j)*sizeof(*data_array)); // dont know if this is redundant...
    // if last (EOF) block size is lesser than the a priori generated block size, dealloc unnecessary memory
    return j; //size_pps
}

void free_data(char **data_array, int size_pps)
{
        for (int j = 0; j < size_pps; j++)
            free(data_array[j]);
        free(data_array);
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
    int flag = 1;

    // reuse address 
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    // deactivate Nagle's algorithm...
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

    // load python arrays
    PyObject *pModuleName, *pModule;  //,,,
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import numpy as np");
    PyRun_SimpleString("sys.path.append(\"../stats/\")");
    pModuleName = PyUnicode_DecodeFSDefault("distargs");
    pModule = PyImport_Import(pModuleName);
    Py_DECREF(pModuleName);
    load_dists(pModule, "ppslow-ubuntu-social-adblock-17-may-alpha-1", "pktlen-ubuntu-30july-03h-dotls-adblock-alpha-0_0", "silence-ubuntu-30july-17h-dotls-alpha-0_001");

    while(eof_flag)
    {
        // generate number of pps to send
        int pps = empirical_prob(pModule, "pps");
        int real_pps;

        // clear servers buffer 
        memset(server_message, '\0', sizeof(server_message));
        
        //init data array 
        char **data_array = (char **) malloc(pps*sizeof(*data_array)); // (char**);   
        
        // create/generate data send
        real_pps = gen_data2send(pModule, data_array, &eof_flag, file_ptr, pps);
       
        printf("\npackets 2 sent: %d", real_pps);
        int small_sleep = empirical_prob(pModule, "low_sil");
        printf("\n SMALL_SLEEP: %d", small_sleep);
        sleep(small_sleep);////
        // FOR LOOP: SEND DATA
        int j;
        for (j = 0; j < real_pps; j++) 
        {
            int str_len = strlen(data_array[j]);
            if ( str_len > 0)
            {
                // overwrite default TCP buffer size 
                setsockopt(socket_desc, SOL_SOCKET, SO_SNDBUF, (int *)&str_len, sizeof(str_len));
//                    setsockopt(socket_desc, SOL_SOCKET, SO_RCVBUF, (int *)&str_len, sizeof(str_len));

                // printf for debug purposes...
                //printf("\nLEN: %d | FD : %d | MESSAGE: %s\n", str_len, socket_desc, data_array[i][j]);  //√√±±

                // write to socket file descriptor 
                write(socket_desc, data_array[j], str_len);

                // flush data in socket file descriptor 
                fflush(socket_file);

            }
        }

        // dealloc/free dynamic(heap) data
        free_data(data_array, real_pps);
    }
    close(socket_desc);
    fclose(file_ptr);
    if (Py_FinalizeEx() < 0)
        return -1;
    return 0;
}


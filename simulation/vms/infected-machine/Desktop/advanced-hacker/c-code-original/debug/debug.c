#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>  //<
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define PI 3.1415926535897932384626433832795
#define EULER 2.7182818284590452353602874713527

// seed generation helper
unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

// generate a random number between 0 and limit...
double random_number(double limit) 
{
    double divisor = (double) RAND_MAX / (limit); // /; //+1
    double number;
    do 
    {
        unsigned long seed = mix(clock(), time(NULL), getpid());
        srand(seed);
        number = ((double)rand()) / divisor;
    } while (number > limit);
    return number;
}

// approximation of combination, computes faster...
long double stirling_approximation(int n)
{
    if (n < 2)
        return 1;
    long double stirling, ans;
    stirling = sqrt(2.0 * PI * n) * pow(n / EULER, n);
    return stirling;
}

long double combination(long long int n, long long int x)
{
    if (x > n / 2)
        x = n - x;

    long double answer = 1;
    for (int i = 1; i <= x; i++)
    {
        answer *= (n - x + i);
        answer /= i;
    }
    return answer;
}

long double binomial(long long int n, long double p, long long int x)
{
    // x = number of sucesses 
    // n = number of trials 
    // P: probability of success of an inividual trial
    long double q = 1 - p;
    long long int nx = n - x;
    //factorial = n! / (n-x)! x!
    //factorial * p^x * q^(n-x)

    return combination(n, x)*pow(p, x)*pow(q, nx);
}

// marsaglia polar method: ziggurat is faster but more complex
double normal_polar(double mean, double std)
{
    static double spare;
    static int has_spare = 0;

    if (has_spare == 1)
    {
        has_spare = 0;
        return spare * std + mean;
    } else 
    {
        double u, v, s;
        do 
        {
            u = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
            v =(rand() / ((double)RAND_MAX)) * 2.0 - 1.0;  
            s = u * u + v * v;
        } while (s >= 1.0 || s == 0.0);
        s = sqrt(-2.0 * log(s) / s);
        spare = v * s;
        has_spare = 1;
        return mean + std * u * s;
    }
}

double gen_normal_weight(double mean, double std, int number, int n)
{
    int sum_exp = 0;
    for (int i = 2; i <= n; i++)
        sum_exp += exp(pow(i-mean, 2) / (2*pow(std, 2)));

    return exp(pow(number-mean, 2)/(2*pow(std, 2))) / sum_exp;
}

int gen_normal_numbers()
{
    double *weights;
    weights = (double *) calloc(48,  sizeof(double *));

    double sum_weights = 0;
    for (int i = 0; i < 48; i++) {
        weights[i] = binomial(200, 0.10, i+2);
        sum_weights = sum_weights + weights[i];
    }
    double rnd = random_number(sum_weights);
    //printf("\nrnd = %f\n", rnd);
    for (int i = 0; i < 48; i++)
    {
        if (rnd < weights[i]) {
            free(weights);
            return i+2;
        }
        rnd -= weights[i];
    }
    free(weights);
}

double gen_exponential_numbers(double lambda)
{
    double u;
    //unsigned long seed = mix(clock(), time(NULL), getpid());
    //srand(seed);

    u = random_number(1);
    return (-log(1-u) / lambda)+1;
}

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

                // alloc 3rd dim of data_array
                data_array[i][j] = (char *) malloc((bytes_to_read+1)*sizeof(char));

                // read "bytes_to_read" bytes from file
                size_t size_read = fread(data_array[i][j], sizeof(char), bytes_to_read, file_ptr);

                // add \0
                data_array[i][j][size_read] = '\0';

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
    FILE *file_ptr = fopen("../../../../files/textfile10k", "rb");  //iii
    int eof_flag = 1;

    // sockets inits
    int socket_desc;
    struct sockaddr_in server_addr;
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
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("111.11.11.10");

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
       
        // FOR LOOP: SEND DATA
        for (int i = 0; i < real_npps; i++) 
        {
            for (int j = 0; j < size_pps[i]; j++) 
            {
                char *msg = "manek>>";
                int str_len = strlen(msg);
                if ( str_len > 0)
                {
                    // overwrite default TCP buffer size 
                    setsockopt(socket_desc, SOL_SOCKET, SO_SNDBUF, (int *)&str_len, sizeof(str_len));

                    // printf for debug purposes...
                    printf("\nMESSAGE: %s\n", msg);  //√√±±

                    // write to socket file descriptor 
                    write(socket_desc, msg, str_len);

                    // flush data in socket file descriptor 
                    fflush(socket_file);

                    ////sleep(0.020);
                }
            }
            //sleep(1.5);
        }
        //sleep(3);

        // dealloc/free dynamic(heap) data
        free_data(data_array, size_pps, real_npps);
    }
    close(socket_desc);
    fclose(file_ptr);
    return 0;
}


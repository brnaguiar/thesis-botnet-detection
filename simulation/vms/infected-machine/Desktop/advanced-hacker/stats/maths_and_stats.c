#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "maths_and_stats.h"

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


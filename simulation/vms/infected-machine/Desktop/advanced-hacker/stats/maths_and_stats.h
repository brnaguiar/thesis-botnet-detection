#ifndef MATHS_AND_STATS_H
#define MATHS_AND_STATS_H

#define PI 3.1415926535897932384626433832795
#define EULER 2.7182818284590452353602874713527

//seed generation helper
unsigned long mix(unsigned long a, unsigned long b, unsigned long c);

// generate a random number between 0 and limit...
double random_number(double limit);

// approximation of combination, computes faster...
long double stirling_approximation(int n);

// compute combinations
long double combination(long long int n, long long int x);

// computes the binomial distribution
long double binomial(long long int n, long double p, long long int x);

// marsaglia polar method: ziggurat is faster, but also more complex...
double normal_polar(double mean, double std);

//
double gen_normal_weight(double mean, double std, int number, int n);

//## generation of normal random numbers for average packet length...
int gen_normal_numbers();

// generation of exponential random numbers for PPS
double gen_exponential_numbers(double lambda);

#endif

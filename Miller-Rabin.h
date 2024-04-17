// Miller-Rabin.h
#ifndef MILLER_RABIN_H
#define MILLER_RABIN_H

#include <stdbool.h>

long long mulmod(long long a, long long b, long long mod);
long long modulo(long long base, long long exponent, long long mod);
bool is_prime(long long n, int iteration);

#endif // MILLER_RABIN_H

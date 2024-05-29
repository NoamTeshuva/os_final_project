#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Miller-Rabin primality test is a probabilistic algorithm 
// used to determine whether a given number is likely to be prime.
// It works by performing a series of modular exponentiations 
// and tests based on Fermat's Little Theorem.

/*
Purpose: 
This function multiplies two numbers a and b under a given modulus mod. 
It helps to avoid overflow issues by using modular arithmetic.

How it works: 
It uses a loop to perform the multiplication, 
adding a to the result only when b is odd, 
and doubling a and halving b each iteration.
 This approach ensures that the intermediate results do not overflow. 
 */

long long mulmod(long long a, long long b, long long mod) {
    // Multiplies two numbers `a` and `b` modulo `mod`.
    // Uses modular arithmetic to prevent overflow.
    long long result = 0;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1)
            result = (result + a) % mod;
        a = (a * 2) % mod;
        b /= 2;
    }
    return result % mod;
}

long long modulo(long long base, long long exponent, long long mod) {
    // Computes `base` raised to the power of `exponent` modulo `mod`.
    // Uses repeated squaring to efficiently compute large powers.
    long long x = 1;
    long long y = base;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}

bool is_prime(long long n, int iteration) {
    // Tests whether the number `n` is likely to be prime 
    // using the Miller-Rabin primality test.
    if (n < 2)
        return false;
    if (n != 2 && n % 2==0)
        return false;
    long long s = n - 1;
    while (s % 2 == 0) {
        s /= 2; //find m for n-1 = (2^k) * m
    }
    for (int i = 0; i < iteration; i++) {
        long long a = rand() % (n - 1) + 1; // choose a
        long long temp = s;
        long long mod = modulo(a, temp, n);
        while (temp != n - 1 && mod != 1 && mod != n - 1) {
            mod = mulmod(mod, mod, n);
            temp *= 2;
        }
        if (mod != n - 1 && temp % 2 == 0) {
            return false;
        }
    }
    return true;
}



/*
if there is a nned to show only the miller_rvabin 
*/
/*
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please provide a number to test\n");
        return 1;
    }
    long long num = atoll(argv[1]);
    srand(time(NULL));
    if (is_prime(num, 5))
        printf("%lld is prime\n", num);
    else
        printf("%lld is not prime\n", num);
    return 0;
}
*/

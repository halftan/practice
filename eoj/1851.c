#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE 98765431

#define deftype long long

void process(int, int);
deftype power(deftype base, deftype power);

int main(void)
{
    int n, t;
    scanf("%d%d", &n, &t);
    process(n, t);
    return 0;
}

void process(int number, int loop) {
    /*
     * The algorithm is:
     * A_ti = S1(m^(T)+1)/n - A_1i if T is even
     * A_ti = S1(m^(T)-1)/n + A_1i if T is odd
     * where S1 is the sum of all initial number
     * and n == number, T == loop
     */
    void sum(int number, deftype *);
    deftype *sums = NULL;
    deftype prefix, mod_inv = 1;
    int i;

    if (number < 2) {
        puts("0");
        return;
    }

    sums = (deftype*) malloc(sizeof(deftype) * (number+1));
    memset(sums, 0, sizeof(deftype) * (number+1));

    for (i = 1; i <= number; ++i)
        scanf("%lld", sums + i);
    sum(number, sums);

    // Modulo multiplicative inverse using Euler's theorem
    mod_inv = power(number, MAX_VALUE - 2);
    prefix = power(number - 1, loop);

    if (loop & 0x01) {
        /* odd number */
        prefix = (prefix + 1) * mod_inv % MAX_VALUE;
    } else {
        /* even number */
        prefix = (1 - prefix) * mod_inv % MAX_VALUE;
    }
    prefix = sums[0] * prefix % MAX_VALUE;
    /* printf("prefix is: %lld\n", prefix); */

    for (i = 1; i <= number; ++i) {
        sums[i] = (sums[i] - prefix) % MAX_VALUE;
        if (loop & 0x01)
            sums[i] = (-sums[i]) % MAX_VALUE;
        if (sums[i] < 0)
            sums[i] += MAX_VALUE;
        printf("%lld\n", sums[i]);
    }

    free(sums); sums = NULL;
}

deftype power(deftype base, deftype pow) {
    deftype ret = 1;
    while (pow > 0) {
        if (pow & 0x01)
            ret = (ret * base) % MAX_VALUE;
        base = (base * base) % MAX_VALUE;
        pow >>= 1;
    }
    return ret;
}

void sum(int number, deftype *array) {
    int i;
    for (i = 1; i <= number; ++i) {
        array[0] += array[i];
        array[0] %= MAX_VALUE;
    }
}

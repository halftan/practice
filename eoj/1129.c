#include <stdio.h>
#include <stdlib.h>

#define TRIG_INDEX(n, k) ( (1 + (n)) * (n) / 2 ) + (k)

void process(int cases);
long long combination(int n, int k);
long long disorder(int k);

int main(void)
{
    int cases;
    scanf("%d", &cases);
    process(cases);
    return 0;
}

void process(int cases)
{
    int n, k;
    while(cases--) {
        scanf("%d %d", &n, &k);
        printf("%lld\n", combination(n, n - k) * disorder(k));
    }
}

long long disorder(int k)
{
    static long long dat[50] = {0,0,1,2};
    static int upper = 3;
    if (upper < k)
        while (upper++ < k)
            dat[upper] = (upper-1) * (dat[upper-1] + dat[upper-2]);
    return dat[k];
}

long long combination(int n, int k)
{
    static int upper_n = 0, upper_k = 0;
    static long long trig[500];
    int indices;

    indices = TRIG_INDEX(n, k);
    if (upper_n > n || (upper_n == n && upper_k >= k)) {
        return trig[indices];
    }

    while (upper_n != n || upper_k != k) {
        if (upper_k != upper_n && upper_k != 0) {
            trig[TRIG_INDEX(upper_n, upper_k)] =
                trig[TRIG_INDEX(upper_n - 1, upper_k - 1)] +
                trig[TRIG_INDEX(upper_n - 1, upper_k)];
        }
        else trig[TRIG_INDEX(upper_n, upper_k)] = 1;
        if (upper_n == upper_k) {
            upper_k = 0; upper_n++;
        }
        else upper_k++;
    }
    if (upper_k != upper_n && upper_k != 0) {
        trig[TRIG_INDEX(upper_n, upper_k)] =
            trig[TRIG_INDEX(upper_n - 1, upper_k - 1)] +
            trig[TRIG_INDEX(upper_n - 1, upper_k)];
    }
    return trig[indices];
}

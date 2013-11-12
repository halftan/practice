#include <stdio.h>

int main(void) {
  long long i;
  i = 10000000000000000;
  while(1) {
    if (i < 0)
      getchar();
    printf("%lld\n", i);
    i += 100000000000000;
  }
}

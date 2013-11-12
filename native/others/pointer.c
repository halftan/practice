#include <stdio.h>
#include <stdlib.h>

int comp(const void *a, const void *b);

typedef struct info {
  int a;
  int b;
} info;

int main(void) {
  info *arr;
  int len, i;
  printf("Input the length of info: ");
  scanf("%i", &len);

  arr = (info*) malloc(sizeof(info) * len);

  for(i = 0; i < len; ++i) {
    scanf("%i%i", &((arr + i)->a), &((arr + i)->b));
  }

  qsort(arr, len, sizeof(info), comp);

  for(i = 0; i < len; ++i) {
    printf("elem %i::\ta: %i\tb: %i\n", i, (arr + i)->a, (arr + i)->b);
  }

  free(arr);
  return 0;
}

int comp(const void *a, const void *b) {
  info *ia, *ib;
  ia = (info*)a;
  ib = (info*)b;

  if(ia->a == ib->a)
    return ia->b - ib->b;
  else
    return ia->a - ib->a;
}

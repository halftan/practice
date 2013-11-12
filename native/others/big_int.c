#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 5000

void get_number(char *dest);
char *add_big_int(char *num_a, char *num_b, char *result);
char *reverse(char *num);
void swap_char(char *src, size_t f, size_t t);
int  min_i(int a, int b);
int  max_i(int a, int b);
int  add_char(char a, char b, char *dest, int step);
void fill_with_zero(char *a, char *b, size_t len);

int main(int argc, char** argv) {

  char result[MAX_LEN];
  char add_a[MAX_LEN];
  char add_b[MAX_LEN];

  while(1) {
    get_number(add_a);
    get_number(add_b);

    add_big_int(add_a, add_b, result);

    printf("The result is: %s\n", result);
  }

  return 0;
}

void get_number(char *dest) {
  scanf("%s", dest);
}

char *add_big_int(char *num_a, char *num_b, char *result) {
  int  i, max_len;
  int  step;

  /* min_len = min_i(strlen(num_a), strlen(num_b)); */
  max_len = max_i(strlen(num_a), strlen(num_b));
  
  /* if(strlen(num_a) > strlen(num_b)) { */
  /*   long_num  = num_a; */
  /*   short_num = num_b; */
  /* } else { */
  /*   long_num  = num_b; */
  /*   short_num = num_a; */
  /* } */

  reverse(num_a);
  reverse(num_b);
  fill_with_zero(num_a, num_b, max_len);

  step = 0;
  for(i = 0; i < max_len; ++i) {
    step = add_char(num_a[i], num_b[i], result + i, step);
  }
  if(step == 1) {
    result[i++] = step + '0';
  }
  result[i] = 0;

  /* for each char a in num_a and b in num_b: */
  /*   c_i != 0 */
  /*     result[c_i] = a + b + result[c_i-1] - 10 */
  /*     result[c_i-1] -= 10 */
  /*   c_i++ */

  return reverse(result);
}

char *reverse(char *num) {
  int  len, i;

  len = strlen(num) - 1;
  for(i = 0; i < (len + 1) / 2; ++i) {
    swap_char(num, i, len - i);
  }

  return num;
}

void swap_char(char *src, size_t f, size_t t) {
  char temp;
  temp = src[f];
  src[f] = src[t];
  src[t] = temp;
}

int max_i(int a, int b) {
  return a > b ? a : b;
}

int min_i(int a, int b) {
  return a > b ? a : b;
}

int add_char(char a, char b, char *dest, int step) {
  printf("Before: %c\t+\t%c\n", a, b);
  *dest = a + b - '0' + step;
  if(*dest > '9') {
    *dest -= (char)10;
    printf("Num: %d\tStep: %d\n", *dest, step);
    return 1;
  }
  printf("Num: %d\tStep: %d\n", *dest, step);
  return 0;
}

void fill_with_zero(char *a, char *b, size_t len) {
  int    i;
  size_t c_len = strlen(a);
  if (c_len < len) {
    for(i = c_len; i < len; ++i) {
      a[i] = '0';
    }
    a[i] = 0;
  }

  c_len = strlen(b);
  if (c_len < len) {
    for(i = c_len; i < len; ++i) {
      b[i] = '0';
    }
    b[i] = 0;
  }
}

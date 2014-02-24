#include <stdio.h>
#include <stdlib.h>

#if defined(__STDC__)
# define PREDEF_STANDARD_C_1989
# if defined(__STDC_VERSION__)
#  define PREDEF_STANDARD_C_1990
#  if (__STDC_VERSION__ >= 199409L)
#   define PREDEF_STANDARD_C_1994
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define PREDEF_STANDARD_C_1999
#  endif
# endif
#endif

#define EMEMORY 4
#define ARRAY_INC 10

#define ARRAY(type) struct {type *arr; size_t count; size_t volume; size_t cell_size;}

#if defined(PREDEF_STANDARD_C_1999)
#   define INIT_ARRAY(type) { .arr=(type*)malloc(sizeof(type)*ARRAY_INC),.count=0, .volume=ARRAY_INC, .cell_size=sizeof(type) }
#else
#   define INIT_ARRAY(type) { (type*)malloc(sizeof(type)*ARRAY_INC), 0, ARRAY_INC, sizeof(type) }
#endif

/*
 * Creates and initializes a new array struct which
 * can be manipulated by other array macros.
 * @type : data type that the array will hold.
 * @name : desired variable name.
 */
#define CREATE_ARRAY(type, name) ARRAY(type) name = INIT_ARRAY(type);

/*
 * Increases the volume of the array.
 * @array : the array to be expanded.
 */
#define ARRAY_EXPAND(array) {array.volume += ARRAY_INC; if (NULL == realloc(array.arr, array.volume*array.cell_size)) {perror(__func__); exit(EMEMORY); }}

/*
 * Push an element to the array.
 * @array : target array struct.
 *          Initialized by INIT_ARRAY macro.
 * @elem : pointer to the element to be pushed into the array.
 * @cpf : the copy function. prototype: void (*fn)(void *src, void *dest)
 */
#define PUSH_BACK3(array, elem, cpf) {if (array.count == array.volume) ARRAY_EXPAND(array) if (cpf == NULL) (array.arr[array.count++]) = elem;else cpf((void*)elem, (void*)array.arr[array.count++]); }

/*
 * 2 argument edition of PUSH_BACK
 */
#define PUSH_BACK(array, elem) {if ((array).count == (array).volume) ARRAY_EXPAND((array)) ((array).arr[array.count++]) = elem; }

/*
 * Traverse the array and do something.
 * @pos : the cursor that will be set to each target element
 *        within each loop.
 * @array : the target array
 */
#define FOREACH(pos, array) for (pos = (array).arr; pos != (array).arr + (array).count; pos++)

/*
 * Traverse the array and do something.
 * Same as FOREACH, but this one has a custom condition.
 * @pos : the cursor that will be set to each target element
 *        within each loop.
 * @array : the target array
 */
#define FOREACH_COND(pos, array, condition) for (pos = (array).arr; condition && pos != (array).arr + (array).count; pos++)

/*
 * Traverse the array and do something.
 * Difference between FOREACH is this macro loop all spaces whether if there's
 * an element in there.
 * This macro may be used to initialize all spaces of the array.
 * @pos : the cursor that will be set to each target element
 *        within each loop.
 * @array : the target array
 */
#define FORALL(pos, array) for (pos = (array).arr; pos != (array).arr + (array).volume; pos++)

/*
 * Get the top element of the array.
 * @array : the target array
 */
#define TOP_ELEM(array) (array).arr + (array).count - ((array.count) == 0 ? 0 : 1)

/*
 * Get the length of the array.
 * @array : target array.
 */
#define LENGTH_OF(array) (array).count

/*
 * Frees memory.
 * @array : the array struct that gets freed.
 */
#define FREE_ARRAY(array) {free((array.arr)); }

#define SWAP(a,b) do{int t;t=a;a=b;b=t;}while(0)

#define remainder(a,b) (a-(a/b)*b)

typedef struct _fraction {
    int   denominator;
    int   numerator;
    float magnitude;
} fraction;

void process(int);
fraction new_fraction(int numerator, int denominator);
int gcd(int a, int b);

int main(void)
{
    int n;
    /* while(scanf("%d", &n) != EOF) */
    scanf("%d", &n);
    process(n);
    return 0;
}

int gcd(int a, int b) {
    int t;
    if (a < b)
        SWAP(a, b);
    while(b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

void process(int n) {
    int comp(const void*, const void*);
    CREATE_ARRAY(fraction, farray);
    int i,j;
    fraction *pos;
    PUSH_BACK(farray, new_fraction(0,1));
    PUSH_BACK(farray, new_fraction(1,1));

    for (j = 2; j <= n; ++j) {
        for (i = 1; i < j; ++i) {
            if (gcd(i,j) == 1) {
                PUSH_BACK(farray, new_fraction(i,j));
            }
        }
    }

    qsort(farray.arr, farray.count, sizeof(fraction), comp);

    FOREACH(pos, farray) {
#if defined(DEBUG)
        printf("%d/%d\t=\t%f\n", pos->numerator, pos->denominator, pos->magnitude);
#else
        printf("%d/%d\n", pos->numerator, pos->denominator);
#endif
    }

    FREE_ARRAY(farray);
}

int comp(const void *a, const void *b) {
    fraction *x, *y;
    float t;
    x = (fraction*)a;
    y = (fraction*)b;
    t =  x->magnitude - y->magnitude;
    if (t < 0.)
        return -1;
    else if (t > 0.)
        return 1;
    else return 0;
}

fraction new_fraction(int numerator, int denominator) {
    fraction ret;
    ret.numerator = numerator;
    ret.denominator = denominator;
    ret.magnitude = (float) numerator / (float) denominator;
    /* printf("new fraction %d/%d: %f\n", ret.numerator, ret.denominator, ret.magnitude); */
    return ret;
}

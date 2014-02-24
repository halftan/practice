#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#define NO_ALLOCS

#ifndef BIGDTYPES_H_
#define BIGDTYPES_H_ 1

#include <stddef.h>

/*
The following PP instructions assume that all Linux systems have a C99-conforming 
<stdint.h>; that other Unix systems have the uint32_t definitions in <sys/types.h>;
and that MS et al don't have them at all. This version assumes that a long is 32 bits.
Adjust if necessary to suit your system. 
You can override by defining HAVE_C99INCLUDES or HAVE_SYS_TYPES.
*/

#ifndef EXACT_INTS_DEFINED_
#define EXACT_INTS_DEFINED_ 1
#ifndef HAVE_C99INCLUDES
	#if (__STDC_VERSION >= 199901L) || defined(linux) || defined(__linux__) || defined(__APPLE__)
	#define HAVE_C99INCLUDES
	#endif
#endif
#ifndef HAVE_SYS_TYPES
	#if defined(unix) || defined(__unix__)
	#define HAVE_SYS_TYPES
	#endif
#endif
#ifdef HAVE_C99INCLUDES
	#include <stdint.h>
#elif defined(HAVE_SYS_TYPES)
	#include <sys/types.h>
#else 
	#define uint32_t unsigned long 
	#define uint16_t unsigned short
	#define uint8_t unsigned char
#endif	/* HAVE_C99INCLUDES */
#endif	/* EXACT_INTS_DEFINED_ */

/* Macros for format specifiers 
-- change to "u", "x" and "X" if necessary */
#ifdef HAVE_C99INCLUDES
	#include <inttypes.h>
#else 
	#define PRIu32 "lu" 
	#define PRIx32 "lx" 
	#define PRIX32 "lX" 
#endif
/* We define our own */
#define PRIuBIGD PRIu32
#define PRIxBIGD PRIx32
#define PRIXBIGD PRIX32

#endif /* BIGDTYPES_H_ */

#ifndef BIGDIGITS_H_
#define BIGDIGITS_H_ 1

#include <stdlib.h>

/**** USER CONFIGURABLE SECTION ****/

/* Define type and size of DIGIT */

/* [v2.1] Changed to use C99 exact-width types. */
/* [v2.2] Put macros for exact-width types in separate file "bigdtypes.h" */

/** The basic BigDigit element, an unsigned 32-bit integer */
typedef uint32_t DIGIT_T;
/** @cond */
typedef uint16_t HALF_DIGIT_T;

/* Sizes to match */
#define MAX_DIGIT 0xFFFFFFFFUL
#define MAX_HALF_DIGIT 0xFFFFUL	/* NB 'L' */
#define BITS_PER_DIGIT 32
#define HIBITMASK 0x80000000UL

/*	[v2.2] added option to avoid allocating temp storage
	and use fixed automatic arrays instead.
	Define NO_ALLOCS to invoke this.
	Only applicable to mp functions. Do not use with bd.
*/
/* Specify the maximum number of digits allowed in a temp mp array
   -- ignored unless NO_ALLOCS is defined */ 
#ifdef NO_ALLOCS
#define MAX_FIXED_DIGITS (8192 / BITS_PER_DIGIT)
#endif

/**** END OF USER CONFIGURABLE SECTION ****/

/**** OPTIONAL PREPROCESSOR DEFINITIONS ****/
/* 
   Choose one of {USE_SPASM | USE_64WITH32}
   USE_SPASM: to use the faster x86 ASM routines (if __asm option is available with your compiler).
   USE_64WITH32: to use the 64-bit integers if available (e.g. long long).
   Default: use default internal routines spDivide and spMultiply.
   The USE_SPASM option takes precedence over USE_64WITH32.
*/

/* Useful macros */
#define ISODD(x) ((x) & 0x1)
#define ISEVEN(x) (!ISODD(x))
#define mpISODD(x, n) (x[0] & 0x1)
#define mpISEVEN(x, n) (!(x[0] & 0x1))


#ifdef __cplusplus
extern "C" {
#endif

/*	
 * Multiple precision calculations	
 * Using known, equal ndigits
 * except where noted
 */

/*************************/
/* ARITHMETIC OPERATIONS */
/*************************/

/** Computes w = u + v, returns carry */
DIGIT_T mpAdd(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits);

/** Computes w = u - v, returns borrow */
DIGIT_T mpSubtract(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits);

/** Computes product w = u * v
@param[out] w To receive the product, an array of size 2 x \c ndigits
@param[in] u An array of size \c ndigits
@param[in] v An array of size \c ndigits
@param[in] ndigits size of arrays \c u and \c v
@warning The product must be of size 2 x \c ndigits
*/
int mpMultiply(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits);

//*************************/
/* COMPARISON OPERATIONS */
/*************************/

/** Returns true (1) if a == b, else false (0) */
int mpEqual(const DIGIT_T a[], const DIGIT_T b[], size_t ndigits);

/** Returns sign of \c {0,1,-1) of \c (a-b) */
int mpCompare(const DIGIT_T a[], const DIGIT_T b[], size_t ndigits);

/** Returns true (1) if a is zero, else false (0) */
int mpIsZero(const DIGIT_T a[], size_t ndigits);

/*************************/
/* ASSIGNMENT OPERATIONS */
/*************************/

/** Sets a = 0 */
DIGIT_T mpSetZero(volatile DIGIT_T a[], size_t ndigits);

/** Sets a = d where d is a single digit */
void mpSetDigit(DIGIT_T a[], DIGIT_T d, size_t ndigits);

/** Sets a = b */
void mpSetEqual(DIGIT_T a[], const DIGIT_T b[], size_t ndigits);

/****************************/
/* SIGNED INTEGER FUNCTIONS */
/****************************/

/** Returns true (1) if x < 0, else false (0) 
@remark Negative numbers are stored in two's-complement representation. Use at your own risk.
*/
int mpIsNegative(const DIGIT_T x[], size_t ndigits);

/**********************/
/* OTHER MP UTILITIES */
/**********************/

/** Returns number of significant non-zero digits in a */
size_t mpSizeof(const DIGIT_T a[], size_t ndigits);

/**************************************/
/* CORE SINGLE PRECISION CALCULATIONS */
/* (double where necessary)           */
/**************************************/

/* NOTE spMultiply and spDivide are used by almost all mp functions. 
   Using the Intel MASM alternatives gives significant speed improvements
   -- to use, define USE_SPASM as a preprocessor directive.
   [v2.2] Removed references to spasm* versions.
*/

/** Computes p = x * y, where x and y are single digits */
int spMultiply(DIGIT_T p[2], DIGIT_T x, DIGIT_T y);

/************************/
/* CONVERSION UTILITIES */
/************************/

/** Converts nbytes octets into big digit a of max size ndigits
@returns actual number of digits set */
size_t mpConvFromOctets(DIGIT_T a[], size_t ndigits, const unsigned char *c, size_t nbytes);
/** Converts big digit a into string of octets, in big-endian order, padding to nbytes or truncating if necessary.
@returns number of non-zero octets required. */
size_t mpConvToOctets(const DIGIT_T a[], size_t ndigits, unsigned char *c, size_t nbytes);
/** Converts a string in decimal format to a big digit.
@returns actual number of (possibly zero) digits set. */
size_t mpConvFromDecimal(DIGIT_T a[], size_t ndigits, const char *s);
/** Converts big digit a into a string in decimal format, where s has size smax including the terminating zero.
@returns number of chars required excluding leading zeroes. */
size_t mpConvToDecimal(const DIGIT_T a[], size_t ndigits, char *s, size_t smax);
/** Converts a string in hexadecimal format to a big digit.
@return actual number of (possibly zero) digits set. */
size_t mpConvFromHex(DIGIT_T a[], size_t ndigits, const char *s);
/** Converts big digit a into a string in hexadecimal format, 
   where s has size smax including the terminating zero.
@return number of chars required excluding leading zeroes. */
size_t mpConvToHex(const DIGIT_T a[], size_t ndigits, char *s, size_t smax);

/** @cond */
/*************************************************************/
/* MEMORY ALLOCATION FUNCTIONS - USED INTERNALLY AND BY BIGD */
/*************************************************************/
/* [v2.2] added option to avoid memory allocation if NO_ALLOCS is defined */
#ifndef NO_ALLOCS
DIGIT_T *mpAlloc(size_t ndigits);
void mpFree(DIGIT_T **p);
#endif
void mpFail(char *msg);

/* Clean up by zeroising and freeing allocated memory */
#ifdef NO_ALLOCS
#define mpDESTROY(b, n) do{if(b)mpSetZero(b,n);}while(0)
#else
#define mpDESTROY(b, n) do{if(b)mpSetZero(b,n);mpFree(&b);}while(0)
#endif
/** @endcond */

#ifdef __cplusplus
}
#endif

#endif	/* BIGDIGITS_H_ */

#define MAX_DIGIT_COUNT 100
#define BUF_SIZE        100

#define S_ZERO "0"
#define S_ONE  "1"
#define S_TWO  "2"
#define S_FIVE "5"

char *process(DIGIT_T [][MAX_DIGIT_COUNT], int, char *buf);

int main(void)
{
    int numbers;
    DIGIT_T dat[200][MAX_DIGIT_COUNT];
    const char *pre[] = {S_ZERO, S_ONE, S_TWO, S_FIVE};
    char *buf = NULL;

    buf = (char*) malloc(sizeof(char) * MAX_DIGIT_COUNT);

    for (numbers = 0; numbers < 4; ++numbers)
        mpConvFromDecimal(dat[numbers], MAX_DIGIT_COUNT, pre[numbers]);

    while (scanf("%d", &numbers) != EOF) {
        printf("%s\n", process(dat, numbers, buf));
    }
    free(buf);

    return 0;
}

char *process(DIGIT_T dat[][MAX_DIGIT_COUNT], int numbers, char *buf)
{
    static int upper = 3;
    static DIGIT_T dbuf[2 * MAX_DIGIT_COUNT];
    static DIGIT_T dbuff[MAX_DIGIT_COUNT];

    while (++upper <= numbers) {
        int left, right;
        left = 0;
        right = upper - 1;
        for(; left <= right; ++left, --right) {
            if (left == 0) {
                mpAdd(dat[upper], dat[right], dat[right], MAX_DIGIT_COUNT);
                /* mpPrintDecimal("Add up ", dat[upper], MAX_DIGIT_COUNT, "\n"); */
            } else {
                mpMultiply(dbuf, dat[left], dat[right], MAX_DIGIT_COUNT);
                if (left != right) {
                    mpAdd(dbuff, dat[upper], dbuf, MAX_DIGIT_COUNT);
                    mpAdd(dat[upper], dbuff, dbuf, MAX_DIGIT_COUNT);
                } else
                    mpAdd(dat[upper], dat[upper], dbuf, MAX_DIGIT_COUNT);
                /* mpPrintDecimal("Add up ", dat[upper], MAX_DIGIT_COUNT, "\n"); */
            }
            /* sum += left == 0 ? 2 * dat[right] : 2 * dat[left] * dat[right]; */
        }
        /* if (numbers % 2 == 1) { */
        /*     mpSetEqual(dbuf, dat[upper], MAX_DIGIT_COUNT); */
        /*     #<{(| mpPrintDecimal("Substract ", dat[(upper - 1) / 2], MAX_DIGIT_COUNT, "\n"); |)}># */
        /*     mpSubtract(dat[upper], dbuf, dat[(upper - 1) / 2], MAX_DIGIT_COUNT); */
        /* } */
            /* sum -= dat[(upper - 1) / 2]; */
    }
    upper--;

    mpConvToDecimal(dat[numbers], MAX_DIGIT_COUNT, buf, BUF_SIZE);
    return buf;
}

/* $Id: bigdigits.c $ */

/******************** SHORT COPYRIGHT NOTICE**************************
This source code is part of the BigDigits multiple-precision
arithmetic library Version 2.4 originally written by David Ireland,
copyright (c) 2001-13 D.I. Management Services Pty Limited, all rights
reserved. It is provided "as is" with no warranties. You may use
this software under the terms of the full copyright notice
"bigdigitsCopyright.txt" that should have been included with this
library or can be obtained from <www.di-mgt.com.au/bigdigits.html>.
This notice must always be retained in any copy.
******************* END OF COPYRIGHT NOTICE***************************/
/*
	Last updated:
	$Date: 2013-04-27 17:19:00 $
	$Revision: 2.4.0 $
	$Author: dai $
*/

/* Core code for BigDigits library "mp" functions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/* For debugging - these are NOOPs */
#define DPRINTF0(s) 
#define DPRINTF1(s, a1) 

/* Flags for preprocessor definitions used (=last digit of mpVersion) */

/* Useful definitions */
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
/* Internal macros */
#define BITS_PER_HALF_DIGIT (BITS_PER_DIGIT / 2)
#define BYTES_PER_DIGIT (BITS_PER_DIGIT / 8)
#define LOHALF(x) ((DIGIT_T)((x) & MAX_HALF_DIGIT))
#define HIHALF(x) ((DIGIT_T)((x) >> BITS_PER_HALF_DIGIT & MAX_HALF_DIGIT))
#define TOHIGH(x) ((DIGIT_T)((x) << BITS_PER_HALF_DIGIT))
#define mpNEXTBITMASK(mask, n) do{if(mask==1){mask=HIBITMASK;n--;}else{mask>>=1;}}while(0)

/****************************/
/* ERROR HANDLING FUNCTIONS */
/****************************/
/* Change these to suit your tastes and operating system. */
#if defined(_WIN32) || defined(WIN32)
/* Win32 GUI alternative */
#ifndef STRICT
#define STRICT
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
void mpFail(char *msg)
{
	MessageBox(NULL, msg, "BigDigits Error", MB_ICONERROR);
	exit(EXIT_FAILURE);
}
#else	/* Ordinary console program */
void mpFail(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
#endif /* _WIN32 */

/*******************************/
/* MEMORY ALLOCATION FUNCTIONS */
/*******************************/
#ifndef NO_ALLOCS
DIGIT_T *mpAlloc(size_t ndigits)
{
	DIGIT_T *ptr;

	/* [v2.3] added check for zero digits. Thanks to "Radistao" */
	if (ndigits < 1) ndigits = 1;

	ptr = (DIGIT_T *)calloc(ndigits, sizeof(DIGIT_T));
	if (!ptr)
		mpFail("mpAlloc: Unable to allocate memory.");

	return ptr;
}

void mpFree(DIGIT_T **p)
{
	if (*p)
	{
		free(*p);
		*p = NULL;
	}
}
#endif /* NO_ALLOCS */

/* Added in [v2.4] for ALLOC_BYTES and FREE_BYTES */
volatile uint8_t zeroise_bytes(volatile void *v, size_t n)
{	/* Zeroise byte array b and make sure optimiser does not ignore this */
	volatile uint8_t optdummy;
	volatile uint8_t *b = (uint8_t*)v;
	while(n--)
		b[n] = 0;
	optdummy = *b;
	return optdummy;
}
/* [v2.4] Added explicit byte allocation functions with and without NO_ALLOCS */
#ifndef NO_ALLOCS
#define ALLOC_BYTES(b,n) do{(b)=calloc(n,1);if(!(b))mpFail("ALLOC_BYTES: Unable to allocate memory.");}while(0)
#define FREE_BYTES(b,n) do{zeroise_bytes((b),(n));free((b));}while(0)
#else
#define MAX_ALLOC_SIZE (MAX_FIXED_DIGITS*BYTES_PER_DIGIT)
#define ALLOC_BYTES(b,n) do{assert((n)<=MAX_ALLOC_SIZE);zeroise_bytes((b),(n));}while(0)
#define FREE_BYTES(b,n) zeroise_bytes((b),(n))
#endif


/* To use, include this statement somewhere in the final code:

	copyright_notice();	
	
It has no real effect at run time. 
Thanks to Phil Zimmerman for this idea.
*/

/**************************************/
/* CORE SINGLE PRECISION CALCULATIONS */
/* (double where necessary)           */
/**************************************/

/* [v2.2] Moved these functions into main file
	and added third option using 64-bit arithmetic if available.
OPTIONS: 
1. define USE_64WITH32 to use 64-bit types on a 32-bit machine; or
2. define USE_SPASM to use Intel ASM (32-bit Intel compilers with __asm support); or
3. use default "long" calculations (any platform)
*/

#ifdef USE_64WITH32
/* 1. We are on a 32-bit machine with a 64-bit type available. */
#pragma message("USE_64WITH32 is set")

/* Make sure we have a uint64_t available */
#if defined (_WIN32) || defined(WIN32)
typedef unsigned __int64 uint64_t;
#elif !defined(HAVE_C99INCLUDES) && !defined(HAVE_SYS_TYPES)
typedef unsigned long long int uint64_t;
#endif

int spMultiply(uint32_t p[2], uint32_t x, uint32_t y)
{
	/* Use a 64-bit temp for product */
	uint64_t t = (uint64_t)x * (uint64_t)y;
	/* then split into two parts */
	p[1] = (uint32_t)(t >> 32);
	p[0] = (uint32_t)(t & 0xFFFFFFFF);

	return 0;
}

#elif defined(USE_SPASM)
/* Use Intel MASM to compute sp products and divisions */
#pragma message("Using MASM")

int spMultiply(uint32_t p[2], uint32_t x, uint32_t y)
/* ASM version explicitly for 32-bit integers */
{
/* Computes p = (p1p0) = x * y. No restrictions on input. */
	__asm
	{
		mov eax, x
		xor edx, edx
		mul y
		; Product in edx:eax
		mov ebx, p
		mov dword ptr [ebx], eax
		mov dword ptr [ebx+4], edx
	}
	return 0;
}

#else
/* Default routines the "long" way */

int spMultiply(DIGIT_T p[2], DIGIT_T x, DIGIT_T y)
{	/*	Computes p = x * y */
	/*	Ref: Arbitrary Precision Computation
	http://numbers.computation.free.fr/Constants/constants.html

         high    p1                p0     low
        +--------+--------+--------+--------+
        |      x1*y1      |      x0*y0      |
        +--------+--------+--------+--------+
               +-+--------+--------+
               |1| (x0*y1 + x1*y1) |
               +-+--------+--------+
                ^carry from adding (x0*y1+x1*y1) together
                        +-+
                        |1|< carry from adding LOHALF t
                        +-+  to high half of p0
	*/
	DIGIT_T x0, y0, x1, y1;
	DIGIT_T t, u, carry;

	/*	Split each x,y into two halves
		x = x0 + B*x1
		y = y0 + B*y1
		where B = 2^16, half the digit size
		Product is
		xy = x0y0 + B(x0y1 + x1y0) + B^2(x1y1)
	*/

	x0 = LOHALF(x);
	x1 = HIHALF(x);
	y0 = LOHALF(y);
	y1 = HIHALF(y);

	/* Calc low part - no carry */
	p[0] = x0 * y0;

	/* Calc middle part */
	t = x0 * y1;
	u = x1 * y0;
	t += u;
	if (t < u)
		carry = 1;
	else
		carry = 0;

	/*	This carry will go to high half of p[1]
		+ high half of t into low half of p[1] */
	carry = TOHIGH(carry) + HIHALF(t);

	/* Add low half of t to high half of p[0] */
	t = TOHIGH(t);
	p[0] += t;
	if (p[0] < t)
		carry++;

	p[1] = x1 * y1;
	p[1] += carry;


	return 0;
}

/* spDivide */

#define B (MAX_HALF_DIGIT + 1)

#endif /* Conditional single-digit mult & div routines */

/************************/
/* ARITHMETIC FUNCTIONS */
/************************/
DIGIT_T mpAdd(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits)
{
	/*	Calculates w = u + v
		where w, u, v are multiprecision integers of ndigits each
		Returns carry if overflow. Carry = 0 or 1.

		Ref: Knuth Vol 2 Ch 4.3.1 p 266 Algorithm A.
	*/

	DIGIT_T k;
	size_t j;

	assert(w != v);

	/* Step A1. Initialise */
	k = 0;

	for (j = 0; j < ndigits; j++)
	{
		/*	Step A2. Add digits w_j = (u_j + v_j + k)
			Set k = 1 if carry (overflow) occurs
		*/
		w[j] = u[j] + k;
		if (w[j] < k)
			k = 1;
		else
			k = 0;
		
		w[j] += v[j];
		if (w[j] < v[j])
			k++;

	}	/* Step A3. Loop on j */

	return k;	/* w_n = k */
}

DIGIT_T mpSubtract(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits)
{
	/*	Calculates w = u - v where u >= v
		w, u, v are multiprecision integers of ndigits each
		Returns 0 if OK, or 1 if v > u.

		Ref: Knuth Vol 2 Ch 4.3.1 p 267 Algorithm S.
	*/

	DIGIT_T k;
	size_t j;

	assert(w != v);

	/* Step S1. Initialise */
	k = 0;

	for (j = 0; j < ndigits; j++)
	{
		/*	Step S2. Subtract digits w_j = (u_j - v_j - k)
			Set k = 1 if borrow occurs.
		*/
		w[j] = u[j] - k;
		if (w[j] > MAX_DIGIT - k)
			k = 1;
		else
			k = 0;
		
		w[j] -= v[j];
		if (w[j] > MAX_DIGIT - v[j])
			k++;

	}	/* Step S3. Loop on j */

	return k;	/* Should be zero if u >= v */
}

int mpMultiply(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], size_t ndigits)
{
	/*	Computes product w = u * v
		where u, v are multiprecision integers of ndigits each
		and w is a multiprecision integer of 2*ndigits

		Ref: Knuth Vol 2 Ch 4.3.1 p 268 Algorithm M.
	*/

	DIGIT_T k, t[2];
	size_t i, j, m, n;

	assert(w != u && w != v);

	m = n = ndigits;

	/* Step M1. Initialise */
	for (i = 0; i < 2 * m; i++)
		w[i] = 0;

	for (j = 0; j < n; j++)
	{
		/* Step M2. Zero multiplier? */
		if (v[j] == 0)
		{
			w[j + m] = 0;
		}
		else
		{
			/* Step M3. Initialise i */
			k = 0;
			for (i = 0; i < m; i++)
			{
				/* Step M4. Multiply and add */
				/* t = u_i * v_j + w_(i+j) + k */
				spMultiply(t, u[i], v[j]);

				t[0] += k;
				if (t[0] < k)
					t[1]++;
				t[0] += w[i+j];
				if (t[0] < w[i+j])
					t[1]++;

				w[i+j] = t[0];
				k = t[1];
			}	
			/* Step M5. Loop on i, set w_(j+m) = k */
			w[j+m] = k;
		}
	}	/* Step M6. Loop on j */

	return 0;
}

/* mpDivide */

int mpEqual(const DIGIT_T a[], const DIGIT_T b[], size_t ndigits)
{
	/*	Returns true if a == b, else false
	*/

	if (ndigits == 0) return -1;

	while (ndigits--)
	{
		if (a[ndigits] != b[ndigits])
			return 0;	/* False */
	}

	return (!0);	/* True */
}

int mpCompare(const DIGIT_T a[], const DIGIT_T b[], size_t ndigits)
{
	/*	Returns sign of (a - b)
	*/

	if (ndigits == 0) return 0;

	while (ndigits--)
	{
		if (a[ndigits] > b[ndigits])
			return 1;	/* GT */
		if (a[ndigits] < b[ndigits])
			return -1;	/* LT */
	}

	return 0;	/* EQ */
}

int mpIsZero(const DIGIT_T a[], size_t ndigits)
{
	/*	Returns true if a == 0, else false
	*/

	size_t i;
	if (ndigits == 0) return -1;

	for (i = 0; i < ndigits; i++)	/* Start at lsb */
	{
		if (a[i] != 0)
			return 0;	/* False */
	}

	return (!0);	/* True */
}

size_t mpSizeof(const DIGIT_T a[], size_t ndigits)
{	/* Returns size of significant digits in a */
	
	while(ndigits--)
	{
		if (a[ndigits] != 0)
			return (++ndigits);
	}
	return 0;
}

size_t mpBitLength(const DIGIT_T d[], size_t ndigits)
/* Returns no of significant bits in d */
{
	size_t n, i, bits;
	DIGIT_T mask;

	if (!d || ndigits == 0)
		return 0;

	n = mpSizeof(d, ndigits);
	if (0 == n) return 0;

	for (i = 0, mask = HIBITMASK; mask > 0; mask >>= 1, i++)
	{
		if (d[n-1] & mask)
			break;
	}

	bits = n * BITS_PER_DIGIT - i;

	return bits;
}

void mpSetEqual(DIGIT_T a[], const DIGIT_T b[], size_t ndigits)
{	/* Sets a = b */
	size_t i;
	
	for (i = 0; i < ndigits; i++)
	{
		a[i] = b[i];
	}
}

 DIGIT_T mpSetZero(volatile DIGIT_T a[], size_t ndigits)
{	/* Sets a = 0 */

	/* Prevent optimiser ignoring this */
	volatile DIGIT_T optdummy;
	volatile DIGIT_T *p = a;

	while (ndigits--)
		a[ndigits] = 0;
	
	optdummy = *p;
	return optdummy;
}

void mpSetDigit(DIGIT_T a[], DIGIT_T d, size_t ndigits)
{	/* Sets a = d where d is a single digit */
	size_t i;
	
	for (i = 1; i < ndigits; i++)
	{
		a[i] = 0;
	}
	a[0] = d;
}

/**********************/
/* BIT-WISE FUNCTIONS */
/**********************/
DIGIT_T mpShiftLeft(DIGIT_T a[], const DIGIT_T *b,
	size_t shift, size_t ndigits)
{	/* Computes a = b << shift */
	/* [v2.1] Modified to cope with shift > BITS_PERDIGIT */
	size_t i, y, nw, bits;
	DIGIT_T mask, carry, nextcarry;

	/* Do we shift whole digits? */
	if (shift >= BITS_PER_DIGIT)
	{
		nw = shift / BITS_PER_DIGIT;
		i = ndigits;
		while (i--)
		{
			if (i >= nw)
				a[i] = b[i-nw];
			else
				a[i] = 0;
		}
		/* Call again to shift bits inside digits */
		bits = shift % BITS_PER_DIGIT;
		carry = b[ndigits-nw] << bits;
		if (bits) 
			carry |= mpShiftLeft(a, a, bits, ndigits);
		return carry;
	}
	else
	{
		bits = shift;
	}

	/* Construct mask = high bits set */
	mask = ~(~(DIGIT_T)0 >> bits);
	
	y = BITS_PER_DIGIT - bits;
	carry = 0;
	for (i = 0; i < ndigits; i++)
	{
		nextcarry = (b[i] & mask) >> y;
		a[i] = b[i] << bits | carry;
		carry = nextcarry;
	}

	return carry;
}

DIGIT_T mpShiftRight(DIGIT_T a[], const DIGIT_T b[], size_t shift, size_t ndigits)
{	/* Computes a = b >> shift */
	/* [v2.1] Modified to cope with shift > BITS_PERDIGIT */
	size_t i, y, nw, bits;
	DIGIT_T mask, carry, nextcarry;

	/* Do we shift whole digits? */
	if (shift >= BITS_PER_DIGIT)
	{
		nw = shift / BITS_PER_DIGIT;
		for (i = 0; i < ndigits; i++)
		{
			if ((i+nw) < ndigits)
				a[i] = b[i+nw];
			else
				a[i] = 0;
		}
		/* Call again to shift bits inside digits */
		bits = shift % BITS_PER_DIGIT;
		carry = b[nw-1] >> bits;
		if (bits) 
			carry |= mpShiftRight(a, a, bits, ndigits);
		return carry;
	}
	else
	{
		bits = shift;
	}

	/* Construct mask to set low bits */
	/* (thanks to Jesse Chisholm for suggesting this improved technique) */
	mask = ~(~(DIGIT_T)0 << bits);
	
	y = BITS_PER_DIGIT - bits;
	carry = 0;
	i = ndigits;
	while (i--)
	{
		nextcarry = (b[i] & mask) << y;
		a[i] = b[i] >> bits | carry;
		carry = nextcarry;
	}

	return carry;
}

int mpSetBit(DIGIT_T a[], size_t ndigits, size_t ibit, int value)
	/* Set bit n (0..nbits-1) with value 1 or 0 */
{
	size_t idigit, bit_to_set;
	DIGIT_T mask;

	/* Which digit? (0-based) */
	idigit = ibit / BITS_PER_DIGIT;
	if (idigit >= ndigits)
		return -1;

	/* Set mask */
	bit_to_set = ibit % BITS_PER_DIGIT;
	mask = 0x01 << bit_to_set;

	if (value)
		a[idigit] |= mask;
	else
		a[idigit] &= (~mask);

	return 0;
}

int mpGetBit(DIGIT_T a[], size_t ndigits, size_t ibit)
	/* Returns value 1 or 0 of bit n (0..nbits-1); or -1 if out of range */
{
	size_t idigit, bit_to_get;
	DIGIT_T mask;

	/* Which digit? (0-based) */
	idigit = ibit / BITS_PER_DIGIT;
	if (idigit >= ndigits)
		return -1;

	/* Set mask */
	bit_to_get = ibit % BITS_PER_DIGIT;
	mask = 0x01 << bit_to_get;

	return ((a[idigit] & mask) ? 1 : 0);
}

void mpModPowerOf2(DIGIT_T a[], size_t ndigits, size_t L)
	/* Computes a = a mod 2^L */
	/* i.e. clears all bits >= L */
{
	size_t i, nw, bits;
	DIGIT_T mask;

	/* High digits to clear */
	nw = L / BITS_PER_DIGIT;
	for (i = nw+1; i < ndigits; i++)
		a[i] = 0;
	/* Low bits to keep */
	bits = L % BITS_PER_DIGIT;
	mask = ~(~0 << bits);
	if (ndigits > nw)
		a[nw] &= mask;
}

void mpXorBits(DIGIT_T a[], const DIGIT_T b[], const DIGIT_T c[], size_t ndigits)
	/* Computes bitwise a = b XOR c */
{
	size_t i;
	for (i = 0; i < ndigits; i++)
		a[i] = b[i] ^ c[i];
}

void mpOrBits(DIGIT_T a[], const DIGIT_T b[], const DIGIT_T c[], size_t ndigits)
	/* Computes bitwise a = b OR c */
{
	size_t i;
	for (i = 0; i < ndigits; i++)
		a[i] = b[i] | c[i];
}

void mpAndBits(DIGIT_T a[], const DIGIT_T b[], const DIGIT_T c[], size_t ndigits)
	/* Computes bitwise a = b AND c */
{
	size_t i;
	for (i = 0; i < ndigits; i++)
		a[i] = b[i] & c[i];
}

void mpNotBits(DIGIT_T a[], const DIGIT_T b[], size_t ndigits)
	/* Computes bitwise a = NOT b */
{
	size_t i;
	for (i = 0; i < ndigits; i++)
		a[i] = ~b[i];
}

/*********************************/
/* FUNCTIONS FOR SIGNED INTEGERS */
/*********************************/
/* Added [v2.2] */

int mpIsNegative(const DIGIT_T x[], size_t ndigits)
	/* Returns TRUE (1) if x < 0, else FALSE (0) */
{
	return ((x[ndigits-1] & HIBITMASK) != 0);
}

/************************/
/* CONVERSION FUNCTIONS */
/************************/
size_t mpConvFromOctets(DIGIT_T a[], size_t ndigits, const unsigned char *c, size_t nbytes)
/* Converts nbytes octets into big digit a of max size ndigits
   Returns actual number of digits set (may be larger than mpSizeof)
*/
{
	size_t i;
	int j, k;
	DIGIT_T t;

	mpSetZero(a, ndigits);

	/* Read in octets, least significant first */
	/* i counts into big_d, j along c, and k is # bits to shift */
	for (i = 0, j = (int)nbytes - 1; i < ndigits && j >= 0; i++)
	{
		t = 0;
		for (k = 0; j >= 0 && k < BITS_PER_DIGIT; j--, k += 8)
			t |= ((DIGIT_T)c[j]) << k;
		a[i] = t;
	}

	return i;
}

size_t mpConvToOctets(const DIGIT_T a[], size_t ndigits, unsigned char *c, size_t nbytes)
/* Convert big digit a into string of octets, in big-endian order,
   padding on the left to nbytes or truncating if necessary.
   Return number of octets required excluding leading zero bytes.
*/
{
	int j, k, len;
	DIGIT_T t;
	size_t i, noctets, nbits;

	nbits = mpBitLength(a, ndigits);
	noctets = (nbits + 7) / 8;

	len = (int)nbytes;

	for (i = 0, j = len - 1; i < ndigits && j >= 0; i++)
	{
		t = a[i];
		for (k = 0; j >= 0 && k < BITS_PER_DIGIT; j--, k += 8)
			c[j] = (unsigned char)(t >> k);
	}

	for ( ; j >= 0; j--)
		c[j] = 0;

	return (size_t)noctets;
}

static size_t uiceil(double x)
/* Returns ceil(x) as a non-negative integer or 0 if x < 0 */
{
	size_t c;

	if (x < 0) return 0;
	c = (size_t)x;
	if ((x - c) > 0.0)
		c++;

	return c;
}

static size_t conv_to_base(const DIGIT_T a[], size_t ndigits, char *s, size_t smax, int base)
/* Convert big digit a into a string in given base format, 
   where s has max size smax.
   Return number of chars set excluding leading zeroes.
   smax can be 0 to find out the required length.
*/
{
#ifdef NO_ALLOCS
	uint8_t bytes[MAX_ALLOC_SIZE], newdigits[MAX_ALLOC_SIZE];
#else
	uint8_t *bytes, *newdigits;
#endif
	const char DEC_DIGITS[] = "0123456789";
	const char HEX_DIGITS[] = "0123456789abcdef";
	size_t newlen, nbytes, nchars;
	size_t n;
	unsigned long t;
	size_t i, j, isig;
	const char *digits;
	double factor;

	switch (base)
	{
	case 10:
		digits = DEC_DIGITS;
		factor = 2.40824;	/* log(256)/log(10)=2.40824 */
		break;
	case 16:
		digits = HEX_DIGITS;
		factor = 2.0;	/* log(256)/log(16)=2.0 */
		break;
	default:
		assert (10 == base || 16 == base);
		return 0;
	}

	/* Set up output string with null chars */
	if (smax > 0 && s)
	{
		memset(s, '0', smax-1);
		s[smax-1] = '\0';
	}

	/* Catch zero input value (return 1 not zero) */
	if (mpIsZero(a, ndigits))
	{
		if (smax > 0 && s)
			s[1] = '\0';
		return 1;
	}

	/* First, we convert to 8-bit octets (bytes), which are easier to handle */
	nbytes = ndigits * BITS_PER_DIGIT / 8;
	ALLOC_BYTES(bytes, nbytes);

	n = mpConvToOctets(a, ndigits, bytes, nbytes);

	/* Create some temp storage for int values */
	newlen = uiceil(n * factor);
	ALLOC_BYTES(newdigits, newlen);

	for (i = 0; i < nbytes; i++)
	{
		t = bytes[i];
		for (j = newlen; j > 0; j--)
		{
			t += (unsigned long)newdigits[j-1] * 256;
			newdigits[j-1] = (unsigned char)(t % base);
			t /= base;
		}
	}

	/* Find index of leading significant digit */
	for (isig = 0; isig < newlen; isig++)
		if (newdigits[isig])
			break;

	nchars = newlen - isig;

	/* Convert to a null-terminated string of decimal chars */
	/* up to limit, unless user has specified null or size == 0 */
	if (smax > 0 && s)
	{
		for (i = 0; i < nchars && i < smax-1; i++)
		{
			s[i] = digits[newdigits[isig+i]];
		}
		s[i] = '\0';
	}

	FREE_BYTES(bytes, nbytes);
	FREE_BYTES(newdigits, newlen);

	return nchars;
}

size_t mpConvToDecimal(const DIGIT_T a[], size_t ndigits, char *s, size_t smax)
/* Convert big digit a into a string in decimal format, 
   where s has max size smax.
   Return number of chars set excluding leading zeroes.
*/
{
	return conv_to_base(a, ndigits, s, smax, 10);
}

size_t mpConvToHex(const DIGIT_T a[], size_t ndigits, char *s, size_t smax)
/* Convert big digit a into a string in hexadecimal format, 
   where s has max size smax.
   Return number of chars set excluding leading zeroes.
*/
{
	return conv_to_base(a, ndigits, s, smax, 16);
}

size_t mpConvFromDecimal(DIGIT_T a[], size_t ndigits, const char *s)
/* Convert a string in decimal format to a big digit.
   Return actual number of digits set (may be larger than mpSizeof).
   Just ignores invalid characters in s.
*/
{
#ifdef NO_ALLOCS
	uint8_t newdigits[MAX_ALLOC_SIZE];
#else
	uint8_t *newdigits;
#endif
	size_t newlen;
	size_t n;
	unsigned long t;
	size_t i, j;
	const int base = 10;

	mpSetZero(a, ndigits);

	/* Create some temp storage for int values */
	n = strlen(s);
	if (0 == n) return 0;
	newlen = uiceil(n * 0.41524);	/* log(10)/log(256)=0.41524 */
	ALLOC_BYTES(newdigits, newlen);

	/* Work through zero-terminated string */
	for (i = 0; s[i]; i++)
	{
		t = s[i] - '0';
		if (t > 9 || t < 0) continue;
		for (j = newlen; j > 0; j--)
		{
			t += (unsigned long)newdigits[j-1] * base;
			newdigits[j-1] = (unsigned char)(t & 0xFF);
			t >>= 8;
		}
	}

	/* Convert bytes to big digits */
	n = mpConvFromOctets(a, ndigits, newdigits, newlen);

	/* Clean up */
	FREE_BYTES(newdigits, newlen);

	return n;
}

size_t mpConvFromHex(DIGIT_T a[], size_t ndigits, const char *s)
/* Convert a string in hexadecimal format to a big digit.
   Return actual number of digits set (may be larger than mpSizeof).
   Just ignores invalid characters in s.
*/
{
#ifdef NO_ALLOCS
	uint8_t newdigits[MAX_ALLOC_SIZE];
#else
	uint8_t *newdigits;
#endif
	size_t newlen;
	size_t n;
	unsigned long t;
	size_t i, j;

	mpSetZero(a, ndigits);

	/* Create some temp storage for int values */
	n = strlen(s);
	if (0 == n) return 0;
	newlen = uiceil(n * 0.5);	/* log(16)/log(256)=0.5 */
	ALLOC_BYTES(newdigits, newlen);

	/* Work through zero-terminated string */
	for (i = 0; s[i]; i++)
	{
		t = s[i];
		if ((t >= '0') && (t <= '9')) t = (t - '0');
		else if ((t >= 'a') && (t <= 'f')) t = (t - 'a' + 10);
		else if ((t >= 'A') && (t <= 'F')) t = (t - 'A' + 10);
		else continue;
		for (j = newlen; j > 0; j--)
		{
			t += (unsigned long)newdigits[j-1] << 4;
			newdigits[j-1] = (unsigned char)(t & 0xFF);
			t >>= 8;
		}
	}

	/* Convert bytes to big digits */
	n = mpConvFromOctets(a, ndigits, newdigits, newlen);

	/* Clean up */
	FREE_BYTES(newdigits, newlen);

	return n;
}

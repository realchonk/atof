#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

float ident (float x)
{
	return x;
}

static float(*volatile black_box)(float) = ident;

float xstrtod (const char *s)
{
	return strtod (s, NULL);
}

float xstrtof (const char *s)
{
	return strtof (s, NULL);
}

float xstrtold (const char *s)
{
	return strtold (s, NULL);
}


float xatof (const char *s)
{
	return atof (s);
}

float empty (const char *s)
{
	(void)s;
	return 0.0f;
}

inline static bool fast_isdigit (int ch)
{
	return ch >= '0' && ch <= '9';
}

#if FAST_ISDIGIT
# define isdigit(ch) fast_isdigit(ch)
#endif

#define def_custom(name, isdigit)			\
float name (const char *s)				\
{							\
	float f = 0.0f, x;				\
	int sign = 1;					\
	int ival = 0, exp;				\
							\
	for (; isspace (*s); ++s);			\
							\
	if (*s == '-') {				\
		sign = -1;				\
		++s;					\
	}						\
							\
	for (ival = 0; isdigit (*s); ++s)		\
		ival = ival * 10 + (*s - '0');		\
							\
	if (*s != '.')					\
		goto skip;				\
	++s;						\
							\
	for (x = 0.1f; isdigit (*s); ++s, x *= 0.1f)	\
		f += (*s - '0') * x;			\
skip:							\
	f = sign * (f + ival);				\
	if (*s != 'e' && *s != 'E')			\
		return f;				\
	++s;						\
							\
	exp = 0;					\
	sign = 1;					\
	if (*s == '-') {				\
		sign = -1;				\
		++s;					\
	}						\
							\
	while (isdigit (*s))				\
		exp = exp * 10 + (*s++ - '0');		\
							\
	return powf (10.0f, sign * exp) * f;		\
}

def_custom(custom, isdigit)
def_custom(custom_fast_isdigit, fast_isdigit)

float dumb (const char *s)
{
	float f = 0.0f, x;
	int ival, sign = 1, exp;

	// sign = '-' ? 1 : -1
	sign = 1 - ((*s++ == '-') * 2);
	ival = *s++ - '0';

	// skip '.'
	++s;

	exp = -1;
	x = 0.1f;
	for (int i = 0; i < 14; ++i, ++s, x *= 0.1f)
		f += (*s - '0') * x;

	// skip 'e'
	++s;

	// skip '-'
	++s;

	exp = 0;
	for (int i = 0; i < 3; ++i)
		exp = exp * 10 + (*s++ - '0');

	return sign * (ival + f) * powf (10.0f, -exp);
}

float custom2 (const char *s)
{
	float f = 0.0f, x;
	int sign = 1, ival = 0, exp;

	for (; isspace (*s); ++s);

	if (*s == '-') {
		sign = -1;
		++s;
	}

	for (ival = 0; isdigit (*s); ++s)
		ival = ival * 10 + (*s - '0');

	if (*s != '.')
		goto skip;
	++s;

	for (x = 0.1f; isdigit (*s); ++s, x *= 0.1f)
		f += (*s - '0') * x;

skip:
	f = sign * (f + ival);

	if (*s != 'e' && *s != 'E')
		return f;
	++s;

	sign = 1;
	if (*s == '-') {
		sign = -1;
		++s;
	}

	for (exp = 0; isdigit (*s); ++s)
		exp = exp * 10 + (*s - '0');

	return powf (10.0f, sign * exp) * f;
}


static float(*parser)(const char *) = FUNC;

int main (int argc, char *argv[]) {
	char line[80];
	(void)argv;

	if (argc == 1) {
		while (fgets (line, sizeof (line), stdin) != NULL)
			black_box (parser (line));
	} else {
		while (fgets (line, sizeof (line), stdin) != NULL)
			printf ("%f\n", parser (line));
	}

	return 0;
}

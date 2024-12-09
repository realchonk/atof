#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <err.h>

#include "pow10f.h"

float ident (float x)
{
	return x;
}
static float(*volatile black_box)(float) = ident;

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

	return pow10f (sign, exp) * f;
}

int main (int argc, char *argv[])
{
	struct stat st;
	char *ptr, *s, *ln;
	int fd = STDIN_FILENO;
	(void)argv;

	if (fstat (fd, &st) != 0)
		err (1, "fstat()");

	if (st.st_size <= 0)
		errx (1, "st.st_size == 0");

	s = ptr = mmap (NULL, st.st_size, PROT_WRITE | PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
		err (1, "mmap()");

	ptr[st.st_size - 1] = '\0';

	if (argc == 1) {
		while ((ln = strsep (&s, "\n")) != NULL) {
			black_box (custom2 (ln));
		}
	} else {
		while ((ln = strsep (&s, "\n")) != NULL) {
			printf ("%f\n", custom2 (ln));
		}
	}

	munmap (ptr, st.st_size);
	return 0;
}

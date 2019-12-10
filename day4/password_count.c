#include <stdio.h>
#include <stdlib.h>

int
valid(int c)
{
	int i, duplicate = 0, last = 10;

	while (c > 0) {
		i = c % 10;
		if (last < i)
			return 0;
		if (last == i)
			duplicate++;
		last = i;
		c /= 10;
	}

	return duplicate;
}

int
complex_valid(int c)
{
	int i, duplicate = 0, runsize = 0, last = 10;

	while (c > 0) {
		i = c % 10;
		if (last < i)
			return 0;
		if (last == i) {
			runsize++;
		} else {
			if (runsize == 2)
				duplicate = 1;
			runsize = 1;
		}
		last = i;
		c /= 10;

	}
	if (runsize == 2)
		return 1;

	return duplicate;
}

int
count_passwords(int start, int end, int *part2)
{
	int count;
	for (count = 0, *part2 = 0; start < end; start++) {
		if (valid(start)) {
			/* printf("%d\n", start); */
			++count;
		}

		if (complex_valid(start))
			++(*part2);
	}

	return count;
}

int
main(int argc, char **argv)
{
	int start, end, part2;

	if (argc < 3) {
		fprintf(stderr, "%s {start} {end}\n", argv[0]);
		return 1;
	}

	start = strtol(argv[1], NULL, 10);
	end = strtol(argv[2], NULL, 10);

	printf("count: %d\n", count_passwords(start, end, &part2));
	printf("count: %d\n", part2);
}

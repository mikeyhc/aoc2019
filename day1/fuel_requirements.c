#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024

int
main(int argc, char **argv)
{
	FILE *fp;
	char buffer[BUFSIZE], *end;
	long sum, value, excess;

	if (argc < 2) {
		printf("%s: input-file\n", argv[0]);
		return 1;
	}

	fp = fopen(argv[1], "r");
	assert(fp);
	sum = excess = 0;
	while (fgets(buffer, BUFSIZE, fp)) {
		value = strtol(buffer, &end, 10) / 3 - 2;
		assert(errno != ERANGE);

		sum += value;
		value = value / 3 - 2;
		while (value > 0) {
			excess += value;
			value = value / 3 - 2;
		}
	}
	fclose(fp);

	printf("first: %ld\n", sum);
	printf("second: %ld\n", sum + excess);


	return 0;
}

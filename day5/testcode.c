#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 4096

int
mem_lookup(const int *memory, int pos)
{
	return memory[memory[pos]];
}

int
read_parameter(const int *memory, int pos, int mode)
{
	if (mode)	/* immediate mode */
		return memory[pos];
	return mem_lookup(memory, pos);
}

void
get_inputs(const int *memory, int pos, int mode1, int mode2, int *in1,
		int *in2)
{
	*in1 = read_parameter(memory, pos, mode1);
	*in2 = read_parameter(memory, pos+1, mode2);
}

void
add(int *memory, int pos, int mode1, int mode2)
{
	int in1, in2;

	get_inputs(memory, pos, mode1, mode2, &in1, &in2);
	memory[memory[pos+2]] = in1 + in2;
}

void
multi(int *memory, int pos, int mode1, int mode2)
{
	int in1, in2;

	get_inputs(memory, pos, mode1, mode2, &in1, &in2);
	memory[memory[pos+2]] = in1 * in2;
}

void
read_input(int *memory, int pos, int *input, int *ipos)
{
	memory[memory[pos]] = input[*ipos];
	++(*ipos);
}

void
write_output(const int *memory, int pos, int mode1)
{
	int in1;

	if (mode1)
		in1 = memory[pos];
	else
		in1 = mem_lookup(memory, pos);

	printf("%d\n", in1);
}

void
less_than(int *memory, int pos, int mode1, int mode2)
{
	int in1, in2;

	get_inputs(memory, pos, mode1, mode2, &in1, &in2);
	if (in1 < in2)
		memory[memory[pos+2]] = 1;
	else
		memory[memory[pos+2]] = 0;
}

void
equals(int *memory, int pos, int mode1, int mode2)
{
	int in1, in2;

	get_inputs(memory, pos, mode1, mode2, &in1, &in2);
	if (in1 == in2)
		memory[memory[pos+2]] = 1;
	else
		memory[memory[pos+2]] = 0;
}

void
print_memory(const int *memory, int max)
{
	int i, j;
	printf("     ");
	for(i = 0; i < 20; i++)
		printf("| %3d ", i);
	printf("|\n-----");
	for(i = 0; i < 20; i++)
		printf("+-----", i);
	printf("+\n");
	for (i = 0; i < max / 20; i++) {
		printf("%4d ", i * 20);
		for (j = 0; j < 20; j++)
			printf("|%5d", memory[i*20+j]);
		printf("|\n-----");
		for(j = 0; j < 20; j++)
			printf("+-----", j);
		printf("+\n");
	}
	printf("\n");
}

void
process(const int *memory, int max, int *input, int input_count)
{
	int working[BUFSIZE], mode1, mode2, mode3, pos = 0, ipos = 0;

	memcpy(working, memory, max * sizeof(int));

	while (pos < max) {
		/* collect instruction modes */
		mode1 = working[pos] / 100 % 10;
		mode2 = working[pos] / 1000 % 100;
		mode3 = working[pos] / 10000;
		assert(!mode3);

		/* handle instructions */
		switch (working[pos++] % 100) {
		case 1:	/* add */
			add(working, pos, mode1, mode2);
			pos += 3;
			break;
		case 2: /* multi */
			multi(working, pos, mode1, mode2);
			pos += 3;
			break;
		case 3: /* input */
			assert(!mode1 && !mode2);
			assert(ipos < input_count);
			read_input(working, pos, input, &ipos);
			++pos;
			break;
		case 4: /* output */
			assert(!mode2);
			write_output(working, pos, mode1);
			++pos;
			break;
		case 5: /* jump-if-true */
			if (read_parameter(working, pos, mode1))
				pos = read_parameter(working, pos+1, mode2);
			else
				pos += 2;
			break;
		case 6: /* jump-if-false */
			if (!read_parameter(working, pos, mode1))
				pos = read_parameter(working, pos+1, mode2);
			else
				pos += 2;
			break;
		case 7: /* less-than */
			less_than(working, pos, mode1, mode2);
			pos += 3;
			break;
		case 8: /* equals */
			equals(working, pos, mode1, mode2);
			pos += 3;
			break;
		case 99: /* exit */
			assert(!mode1 && !mode2);
			return;
		default:
			print_memory(working, max);
			fprintf(stderr, "invalid instruction: %d(%d)\n",
					working[pos-1], pos-1);
			abort();
		}

	}

	fprintf(stderr, "ran off end of instructions\n");
	abort();
}

int
main(int argc, char **argv)
{
	char input[BUFSIZE], *next, *end;
	int memory[BUFSIZE], ac_input[] = {1}, tr_input[] = {5};
	int pos = 0;

	while (fgets(input, BUFSIZE, stdin)) {
		next = input;
		while (*next && *next != '\n') {
			memory[pos++] = strtol(next, &end, 10);
			if (*end == ',')
				end++;
			next = end;
		}
	}

	printf("Air Conditioning\n");
	process(memory, pos, ac_input, 1);
	printf("\nThermal Radiators\n");
	process(memory, pos, tr_input, 1);

	return 0;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024
#define TARGET 19690720

int
mem_lookup(int *memory, int pos)
{
	return memory[memory[pos]];
}

int
process(const int *memory, int max, int val1, int val2)
{
	int working[BUFSIZE];
	int pos = 0;

	memcpy(working, memory, max * sizeof(int));

	working[1] = val1;
	working[2] = val2;
	while (pos < max && memory[pos] != 99) {
		assert(pos + 3 < max);
		assert(memory[pos + 3] < max);
		switch (memory[pos]) {
		case 1:
			working[working[pos+3]] = mem_lookup(working, pos+1) +
				mem_lookup(working, pos+2);
			break;
		case 2:
			working[working[pos+3]] = mem_lookup(working, pos+1) *
				mem_lookup(working, pos+2);
			break;
		default:
			printf("error: invalid opcode(%d)\n", memory[pos]);
			exit(1);
		}
		pos += 4;
	}

	return working[0];
}

int
main(int argc, char **argv)
{
	char input[BUFSIZE], *next, *end;
	int memory[BUFSIZE];
	int pos = 0, base, offset1, offset2, noun, verb, check;

	while (fgets(input, BUFSIZE, stdin)) {
		next = input;
		while (*next && *next != '\n') {
			memory[pos++] = strtol(next, &end, 10);
			if (*end == ',')
				end++;
			next = end;
		}
	}

	// part one
	printf("memory[0]: %d\n", process(memory, pos, 12, 2));

	// part two
	// determine offsets for noun and verb
	base = process(memory, pos, 0, 0);
	offset1 = process(memory, pos, 1, 0) - base;
	offset2 = process(memory, pos, 0, 1) - base;
	printf("base: %d; offset[1]: %d; offset[2]: %d\n", base, offset1,
			offset2);
	// this assumes verb cleanly divides the remainder after noun
	// which was true for my input, but possibly not globally
	noun = (TARGET - base) / offset1;
	verb = (TARGET - base - noun * offset1) / offset2;

	check = process(memory, pos, noun, verb);

	printf("check: %d == %d (%s)\n", TARGET, check,
			check == TARGET ? "true" : "false");
	printf("output: %d\n", 100 * noun + verb);

	return 0;
}

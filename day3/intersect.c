#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 2048
#define LISTSIZE 256

typedef struct {
	char dir;
	int pos1, pos2;
	int steps;
	int idx;
} Segment;

void
update_segment(Segment *s, char dir, int pos1, int pos2, int idx, int steps)
{
	s->dir = dir;
	s->pos1 = pos1;
	s->pos2 = pos2;
	s->idx = idx;
	s->steps = steps;
}

void
read_instructions(Segment *vertical, Segment *horizontal, int *vsize,
		int *hsize)
{
	int curx = 0, cury = 0, dist, steps = 0;
	char buffer[BUFSIZE], dir, *start, *end;

	*vsize = 0;
	*hsize = 0;
	assert(fgets(buffer, BUFSIZE, stdin));

	start = buffer;
	do {
		/* parse a single section */
		dir = *start++;
		dist = strtol(start, &end, 10);
		if (*end == ',')
			++end;
		start = end;

		/* do the "movement" */
		switch (dir) {
		case 'R':
			update_segment(horizontal + *hsize, dir, curx,
					curx + dist, cury, steps);
			curx += dist;
			++(*hsize);
			break;
		case 'L':
			update_segment(horizontal +*hsize, dir, curx - dist,
					curx, cury, steps);
			curx -= dist;
			++(*hsize);
			break;
		case 'U':
			update_segment(vertical + *vsize, dir, cury,
					cury + dist, curx, steps);
			cury += dist;
			++(*vsize);
			break;
		case 'D':
			update_segment(vertical + *vsize, dir, cury - dist,
					cury, curx, steps);
			cury -= dist;
			++(*vsize);
			break;
		default:
			printf("invalid direction: %c\n", dir);
			exit(1);
		}
		steps += dist;
	} while (*end && *end != '\n');

	printf("read %d horizontal and %d vertical lines\n", *hsize, *vsize);
}

int
in(Segment a, Segment b)
{
	return a.idx >= b.pos1 && a.idx <= b.pos2 &&
		b.idx >= a.pos1 && b.idx <= a.pos2;
}

int
distance(Segment a, Segment b)
{
	return abs(a.idx) + abs(b.idx);
}

void
get_orig(Segment s, int *x, int *y)
{
	switch (s.dir) {
	case 'U':
		*x = s.idx;
		*y = s.pos1;
		break;
	case 'D':
		*x = s.idx;
		*y = s.pos2;
		break;
	case 'R':
		*x = s.pos1;
		*y = s.idx;
		break;
	case 'L':
		*x = s.pos2;
		*y = s.idx;
		break;
	default:
		printf("invalid direction: %c\n", s.dir);
		exit(1);
	}
}

int
steps(Segment a, Segment b)
{
	int ax, ay, bx, by;

	get_orig(a, &ax, &ay);
	get_orig(b, &bx, &by);

	return a.steps + b.steps + abs(bx - ax) + abs(by - ay);
}

/* this is relatively inefficient, but the dataset is small so not worth
 * implementing anything faster */
int
intersect(Segment *vertical, Segment *horizontal, int vsize, int hsize,
		int *min_steps)
{
	int i, j, min = INT_MAX, d, s;

	*min_steps = INT_MAX;
	for (i = 0; i < vsize; ++i) {
		for (j = 0; j < hsize; ++j) {
			if (in(vertical[i], horizontal[j])) {
				d = distance(vertical[i], horizontal[j]);
				if (d > 0)
					min = d < min ? d : min;
				s = steps(vertical[i], horizontal[j]);
				if (s > 0)
					*min_steps = s < *min_steps ? s
						:*min_steps;
			}
		}
	}

	return min;
}

int
main(int argc, char **argv)
{
	Segment v1[LISTSIZE], h1[LISTSIZE], v2[LISTSIZE], h2[LISTSIZE];
	int d1, d2, m1, m2, v1size, h1size, v2size, h2size;

	read_instructions(v1, h1, &v1size, &h1size);
	read_instructions(v2, h2, &v2size, &h2size);

	d1 = intersect(v1, h2, v1size, h2size, &m1);
	d2 = intersect(v2, h1, v2size, h1size, &m2);

	printf("min distance: %d\n", d1 < d2 ? d1 : d2);
	printf("min steps: %d\n", m1 < m2 ? m1 : m2);

	return 0;
}

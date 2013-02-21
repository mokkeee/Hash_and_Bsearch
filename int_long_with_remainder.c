#include <stdio.h>
#include <sys/time.h>

int k = 31;
long kl = 31;
int n = 10001;
static unsigned int get_int(unsigned int i) {
	return (i * k) % n;
}
static unsigned long get_long(unsigned long l) {
	return (l * kl) % n;
}

static struct timeval get_timediff(struct timeval* p_start, struct timeval* p_end) {
	struct timeval ret;
	if ( p_start->tv_usec <= p_end->tv_usec ) {
		ret.tv_sec = p_end->tv_sec - p_start->tv_sec;
		ret.tv_usec = p_end->tv_usec - p_start->tv_usec;
	}
	else {
		ret.tv_sec = p_end->tv_sec - p_start->tv_sec - 1;
		ret.tv_usec = 1000000 + p_end->tv_usec - p_start->tv_usec;
	}

	return ret;
}

#define LOOP_MIN  0x7fffffff
#define LOOP_MAX  0xffffffff
volatile unsigned int v;
volatile unsigned long vl;

void check_int() {
	struct timeval start, end, diff;
	unsigned int i;
	
	gettimeofday(&start, NULL);
	for ( i=LOOP_MIN; i<LOOP_MAX; i++ ) {
		v = get_int(i);
		//if( get_int(i) == 0xffffffff ) printf("get_int start\n");
	}
	gettimeofday(&end, NULL);
	diff = get_timediff(&start, &end);
	printf("get_int  : %u.%06u\n", diff.tv_sec, diff.tv_usec);
}

void check_long() {
	struct timeval start, end, diff;
	unsigned long l;
	
	gettimeofday(&start, NULL);
	for ( l=LOOP_MIN; l<LOOP_MAX; l++ ) {
		vl = get_long(l);
		//if( get_long(l) == 0xffffffff ) printf("get_long start\n");
	}
	gettimeofday(&end, NULL);
	diff = get_timediff(&start, &end);
	printf("get_long : %u.%06u\n", diff.tv_sec, diff.tv_usec);
}

int main()
{
	check_int();
	check_long();
	check_int();
	check_long();
	return 0;
}

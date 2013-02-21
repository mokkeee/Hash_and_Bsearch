/*
 * search_main.c
 *
 *  Created on: 2013/02/04
 *      Author: motegi
 */

#include <sys/time.h>

#include "search_sample.h"

/**
 * ２つの時刻の差分を取得する
 * @param p_start 前の時間
 * @param p_end 後の時間
 * @return
 */
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

//#define SEARCH_COUNT_PER_1DATA 1
#define SEARCH_COUNT_PER_1DATA 10000

extern struct ipdata* p_bsearch_data;
extern int data_count;

int main(int argc, char* p_argv[]) {
	int n,k;

	if ( argc != 4 ) {
		fprintf(stderr, "Usage: %s IPv4address-filename hash-N hash-k\n", p_argv[0]);
		return 1;
	}

	n = atoi(p_argv[2]);
	k = atoi(p_argv[3]);

	// データ初期化
	if ( init_bsearch(p_argv[1]) != OK ) return 2;
	if ( init_hsearch(p_argv[1], n, k) != OK ) return 2;
#ifdef DEBUG
    output_data("/tmp/tmp.txt");
#endif /* DEBUG */

	// 性能測定実施
	// 読み込んだ全てのデータについて、各データをSEARCH_COUNT_PER_1DATA回ずつ検索し、
	// その検索のトータル時間を測定する
    struct timeval start_time, end_time, diff_time;
    struct ipdata* p_target;
    int     notfound_count;
    int		i;

	// ２分検索
    notfound_count = 0;
    gettimeofday(&start_time, NULL);
	for ( i = 0, p_target = p_bsearch_data;
			i < data_count;
			p_target = &(p_bsearch_data[++i]) )
	{
        int i;
		for ( i = 0; i < SEARCH_COUNT_PER_1DATA; i++ ) {
			if ( execute_bsearch(p_target) != p_target ) {
#ifdef DEBUG
                char	dst[INET_ADDRSTRLEN];
				printf("error ip=(%u) %s\n", p_target->addr.s_addr, inet_ntop(AF_INET, p_target, dst, sizeof(dst)));
#endif /* DEBUG */
				notfound_count++;
			}
		}
	}
    gettimeofday(&end_time, NULL);
    diff_time = get_timediff(&start_time, &end_time);
    printf("二分検索時間,%d.%06d,", diff_time.tv_sec, diff_time.tv_usec);
    if ( notfound_count != 0 ) {
    	fprintf(stderr, "binary search error. data not found. count=%d\n", notfound_count);
    }

	// ハッシュ検索
    notfound_count = 0;
    gettimeofday(&start_time, NULL);
	for ( i = 0, p_target = p_bsearch_data;
			i < data_count;
			p_target = &p_bsearch_data[++i] )
	{
        int i;
		for ( i = 0; i < SEARCH_COUNT_PER_1DATA; i++ ) {
			if ( execute_hsearch(p_target) == NULL ) {
				notfound_count++;
			}
		}
	}
    gettimeofday(&end_time, NULL);
    diff_time = get_timediff(&start_time, &end_time);
    printf("ハッシュ検索時間,%d.%06d\n", diff_time.tv_sec, diff_time.tv_usec);
    if ( notfound_count != 0 ) {
    	fprintf(stderr, "hash search error. data not found. count=%d\n", notfound_count);
    }
}


/** ファイルのデータ行数をカウントする */
int count_data(const char* p_filename) {
	FILE*	p_fp;
	char	line[1000];
	int		line_cnt = 0;

	p_fp = fopen(p_filename, "r");
	if ( p_fp == NULL ) {
		perror("fopen error.");
		exit(5);
	}

	while (NULL != fgets(line, sizeof(line), p_fp)) {
		// 空行は無視
		if ( line[0] == '\n' ) continue;
		if ( line[0] == '\0' ) continue;

		line_cnt++;
	}
	fclose(p_fp);

	return line_cnt;
}

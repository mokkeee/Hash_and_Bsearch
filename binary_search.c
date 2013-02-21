/*
 * binary_search.c
 *
 *  Created on: 2013/02/04
 *      Author: motegi
 */

#include "search_sample.h"

struct ipdata* p_bsearch_data;
int data_count;

/** 比較用関数 */
static int compare(const struct ipdata* p1, const struct ipdata* p2) {
	if (p1->addr.s_addr > p2->addr.s_addr) return 1;
	else if (p1->addr.s_addr < p2->addr.s_addr) return -1;
    else return 0;
}

/** 2分検索用データ作成 */
int init_bsearch(const char* p_filename) {
    int bsearch_data_size;

	data_count = count_data(p_filename);
	bsearch_data_size = sizeof(p_bsearch_data[0]) * data_count;
    p_bsearch_data = malloc(bsearch_data_size);

	FILE*	p_fp;
	char	line[1000];
	int		i = 0;

	p_fp = fopen(p_filename, "r");
	if ( p_fp == NULL ) {
		perror("fopen error.");
		return NG;
	}

	while (NULL != fgets(line, sizeof(line), p_fp)) {
		// 空行は無視
        switch(line[0]) {
        case '\n':
        case '\0':
        	continue;
        }

		// 取得したデータを検索用データとして登録
        line[strlen(line)-1] = '\0';
        inet_pton(AF_INET, line, &p_bsearch_data[i].addr);
        i++;
	}
	fclose(p_fp);

    // data sort
	qsort(p_bsearch_data, data_count, sizeof(p_bsearch_data[0]),
			(int(*)(const void*, const void*))compare);

	return OK;
}

/** 2分検索実施 */
struct ipdata* execute_bsearch(const struct ipdata* p_target) {
	return (struct ipdata*)bsearch(p_target, p_bsearch_data,
            		data_count, sizeof(p_bsearch_data[0]),
					(int(*)(const void*, const void*))compare);
}


void output_data(char* p_output_filename) {
	FILE*	p_fp;
	p_fp = fopen(p_output_filename, "w");
	if ( p_fp == NULL ) {
		perror("fopen error.");
		return;
	}

	int i;
	for ( i = 0; i < data_count; i++ ) {
		fprintf(p_fp, "%u\n", (uint32_t)p_bsearch_data[i].addr.s_addr);
	}
	fclose(p_fp);
}

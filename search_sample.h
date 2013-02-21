/*
 * search_sample.h
 *
 *  Created on: 2013/02/04
 *      Author: motegi
 */

#ifndef SEARCH_SAMPLE_H_
#define SEARCH_SAMPLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>


/** OK/NG */
enum {
	OK,
	NG,
};

/** IPv4データ */
struct ipdata {
	struct in_addr addr;
};

/** 各ハッシュ値に対するハッシュデータ */
struct hash_branch {
	struct ipdata data;
	struct hash_branch *p_next;
};

/** ハッシュのメインテーブル */
struct hash_base {
	int		hash_n;
	struct hash_branch* p_branch[];
};


int count_data(const char* p_filename);

// 2分検索用API
extern int init_bsearch(const char* p_filename);
extern struct ipdata* execute_bsearch(const struct ipdata* p_target);
void output_data(char* p_output_filename);

// ハッシュ検索用API
extern int init_hsearch(const char* p_filename, int n, int k);
extern struct ipdata* execute_hsearch(const struct ipdata* p_target);

#endif /* SEARCH_SAMPLE_H_ */

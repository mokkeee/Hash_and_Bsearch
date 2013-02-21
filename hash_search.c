/*
 * hash_search.c
 *
 *  Created on: 2013/02/04
 *      Author: motegi
 */

#include "search_sample.h"

static unsigned int hash_size_n;
static unsigned int	 hash_coefficient_k;
struct hash_base* p_hash_data;

/** ハッシュ値計算
 *
 * @param addrval IPv4アドレス値（ネットワークバイトオーダ）
 * @return ハッシュ値
 */
static uint32_t get_hash(uint32_t addrval) {
	// 桁落ちが発生しないようにunsigned longにキャストすると性能が落ちる
	unsigned long tmp = addrval * hash_coefficient_k;
	return (uint32_t)(tmp % hash_size_n);
	//return (uint32_t)((addrval * hash_coefficient_k) % hash_size_n);
}

/**
 * ハッシュテーブル作成
 * @param p_filename IPv4アドレスファイル
 * @param n ハッシュテーブルサイズ
 * @param k ハッシュ係数
 * @return
 */
int init_hsearch(const char* p_filename, int n, int k) {
	int data_count;
	int hashtable_size;

	data_count = count_data(p_filename);
	hashtable_size = sizeof(*p_hash_data) + sizeof(p_hash_data[0]) * n;
    p_hash_data = malloc(hashtable_size);
    hash_size_n = n;
    hash_coefficient_k = k;

    memset(p_hash_data, 0, hashtable_size);

	FILE*	p_fp;
	char	line[1000];
	int		i = 0;

	p_fp = fopen(p_filename, "r");
	if ( p_fp == NULL ) {
		perror("fopen error.");
		return NG;
	}

	// ハッシュにデータ登録
	while (NULL != fgets(line, sizeof(line), p_fp)) {
        uint32_t	hash_value;
        struct in_addr read_addr;
        struct hash_branch**		pp_target;

		// 空行は無視
        switch(line[0]) {
        case '\n':
        case '\0':
        	continue;
        }

		// 取得したデータを検索用データとして登録
        line[strlen(line)-1] = '\0';
        inet_pton(AF_INET, line, &read_addr);
        hash_value = get_hash(read_addr.s_addr);

        // ハッシュ登録位置取得
        // シノニム発生時は、登録済みデータリストの最後に登録
        for (pp_target = &p_hash_data->p_branch[hash_value];
        		*pp_target != NULL;
        		pp_target = &((*pp_target)->p_next));

        *pp_target = malloc(sizeof(**pp_target));
        (*pp_target)->data.addr = read_addr;
        (*pp_target)->p_next = NULL;
	}
	fclose(p_fp);

	return OK;
}


/** ハッシュ検索実施
 *
 * @param p_target 検索対象データ
 * @return 検索データ
 */
struct ipdata* execute_hsearch(const struct ipdata* p_target) {
    uint32_t hashval = get_hash(p_target->addr.s_addr);
    struct hash_branch** pp_data;

    // データ検索
    for ( pp_data = &(p_hash_data->p_branch[hashval]);
    		*pp_data != NULL;
    		pp_data = &((*pp_data)->p_next) ) {
    	if ( (*pp_data)->data.addr.s_addr == p_target->addr.s_addr ) {
    		return &((*pp_data)->data);
    	}
    }

    return NULL;
}

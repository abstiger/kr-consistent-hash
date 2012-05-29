#ifndef __KR_SKIPLIST_H__
#define __KR_SKIPLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZSKIPLIST_MAXLEVEL 12   
#define ZSKIPLIST_BRANCHING 3   

typedef int (*KRCompareFunc)(const void *a, const void *b);

/* ZSETs use a specialized version of Skiplists */
typedef struct _kr_skiplist_node_t {
    unsigned int key;
    void      *value;
    struct _kr_skiplist_node_t **forward;
}T_KRSkipListNode;

typedef struct _kr_skiplist_t {
    struct _kr_skiplist_node_t *header;
    unsigned int length;
    int level;
    KRCompareFunc compare_func;
}T_KRSkipList;


T_KRSkipListNode *kr_skiplist_create_node(int level, unsigned int key, void *value);
void kr_skiplist_free_node(T_KRSkipListNode *node);

T_KRSkipList *kr_skiplist_create(KRCompareFunc kr_compare_func);
void kr_skiplist_free(T_KRSkipList *zsl) ;

T_KRSkipListNode *kr_skiplist_insert(T_KRSkipList *zsl, unsigned int key, void *value);
int kr_skiplist_delete(T_KRSkipList *zsl, unsigned int key, void *value);

void *kr_skiplist_lookup(T_KRSkipList *zsl, unsigned int key);


#endif /* __KR_SKIPLIST_H__ */
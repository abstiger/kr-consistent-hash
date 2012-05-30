#ifndef __KR_SKIPLIST_H__
#define __KR_SKIPLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZSKIPLIST_MAXLEVEL 12   
#define ZSKIPLIST_BRANCHING 4   

typedef int (*KRCompareFunc)(const void *a, const void *b);
typedef void (*KRForEachFunc)(void *value, void *data);

/* skiplist node definition */
typedef struct _kr_skiplist_node_t {
    unsigned int key;
    void      *value;
    struct _kr_skiplist_node_t **forward;
}T_KRSkipListNode;

/* skiplist definition */
typedef struct _kr_skiplist_t {
    struct _kr_skiplist_node_t *header;
    unsigned int length;
    int level;
    KRCompareFunc compare_func;
}T_KRSkipList;


T_KRSkipListNode *kr_skiplist_create_node(int level, unsigned int key, void *value);
void kr_skiplist_free_node(T_KRSkipListNode *node);

T_KRSkipList *kr_skiplist_create(KRCompareFunc compare_func);
void kr_skiplist_free(T_KRSkipList *krsl) ;

T_KRSkipListNode *kr_skiplist_insert(T_KRSkipList *krsl, unsigned int key, void *value);
int kr_skiplist_delete(T_KRSkipList *krsl, unsigned int key, void *value);
void *kr_skiplist_lookup(T_KRSkipList *krsl, unsigned int key, void *value);
void kr_skiplist_foreach(T_KRSkipList *krsl, KRForEachFunc foreach_func, void *data);

/* cycle look up for consistent hash locating */
void *kr_skiplist_lookup_cycle(T_KRSkipList *krsl, unsigned int key);

#endif /* __KR_SKIPLIST_H__ */
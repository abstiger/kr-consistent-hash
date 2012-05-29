#ifndef __KR_CONHASH_H__
#define __KR_CONHASH_H__

#include "kr_skiplist.h"

typedef unsigned int (*KRHashFunc)(const void *key);

typedef struct _kr_virtual_node_t T_KRVirtualNode;

typedef enum {
    KR_NODE_STATUS_CREATED = '0',
    KR_NODE_STATUS_INSERTED,
    KR_NODE_STATUS_DELETED
}E_KRNodeStatus;

/* actual node structure */
typedef struct _kr_actual_node_t
{
	char            id[100];     /*node identifier, should be unique!*/
	unsigned int    weights;     /*node weights, the replicas of this node*/
	T_KRVirtualNode *vnodes;    /*vitual nodes of this actual node*/
	E_KRNodeStatus  status;      /*node status*/
}T_KRActualNode;

/* virtual node structure */
struct _kr_virtual_node_t
{
	unsigned int    hash;
	char            vid[128];    /* vnode identifier, come from nodeid!*/
	T_KRActualNode *node;        /* pointer to the actual node */
};

/* consistent hashing */
typedef struct _kr_conhash_t
{
	T_KRSkipList   *vnode_list;  /* skiplist of virtual nodes */
	unsigned int    mod;         /* hash mod  */
	unsigned int    node_num;    /* actual nodes number */
	KRHashFunc      hash_func;   /* hash function */
}T_KRConHash;



T_KRConHash *kr_conhash_construct(unsigned int mod, KRHashFunc hash_func);
void kr_conhash_destruct(T_KRConHash *krch);

T_KRActualNode *kr_conhash_create_node(char *id, unsigned int weights);
void kr_conhash_free_node(T_KRActualNode *node);

int kr_conhash_insert(T_KRConHash *krch, T_KRActualNode *node);
int kr_conhash_delete(T_KRConHash *krch, T_KRActualNode *node);
T_KRActualNode *kr_conhash_locate(T_KRConHash *krch, char *object);

#endif /* __KR_CONHASH_H__ */

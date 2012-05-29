#include "kr_conhash.h"

/* the default compare function */
static inline int kr_vnode_compare(const void * v1, const void * v2)
{
    T_KRVirtualNode *vnode1 = (T_KRVirtualNode *)v1;
    T_KRVirtualNode *vnode2 = (T_KRVirtualNode *)v2;
  
    return strcmp(vnode1->node->id, vnode2->node->id);
}


/* the default hash function */
static inline unsigned int kr_string_hash(const void *v)
{
    const signed char *p;
    unsigned int h = 5381;

    for (p = v; *p != '\0'; p++)
        h = (h << 5) + h + *p;

    return h;
}

T_KRConHash *
kr_conhash_construct(unsigned int mod, KRHashFunc hash_func)
{
    T_KRConHash *krch = malloc(sizeof(T_KRConHash));
    if (krch == NULL) {
        fprintf(stderr, "malloc krch failed!\n");
        return NULL;
    }
    
    krch->vnode_list = kr_skiplist_create(kr_vnode_compare);
    if (krch->vnode_list == NULL) {
        fprintf(stderr, "kr_skiplist_create failed!\n");
        return NULL;
    }
    
    krch->mod = mod;
    krch->node_num = 0;
    krch->hash_func = hash_func?hash_func:kr_string_hash;
    
    return krch;
}


void kr_conhash_destruct(T_KRConHash *krch)
{
    kr_skiplist_free(krch->vnode_list);
    free(krch);
}


T_KRActualNode *kr_conhash_create_node(char *id, unsigned int weights)
{
    T_KRActualNode *node = malloc(sizeof(T_KRActualNode));
    if (node == NULL) {
        fprintf(stderr, "malloc node failed!\n");
        return NULL;
    }
    
    strncpy(node->id, id, sizeof(node->id));
    node->weights = weights;
    node->vnodes = calloc(weights, sizeof(T_KRVirtualNode));
    if (node->vnodes == NULL) {
        fprintf(stderr, "malloc node->node_hashes failed!\n");
        return NULL;
    }
    node->status = KR_NODE_STATUS_CREATED;
    
    return node;
}


void kr_conhash_free_node(T_KRActualNode *node)
{
    free(node->vnodes);
    free(node);
}


int kr_conhash_insert(T_KRConHash *krch, T_KRActualNode *node)
{
    int i;
    T_KRVirtualNode *vnode = NULL;
    
    for (i = 0; i < node->weights; i++) {
        vnode = &node->vnodes[i];
        
        snprintf(vnode->vid, sizeof(vnode->vid), "%s-%03d", node->id, i);
        vnode->hash = krch->hash_func(vnode->vid) % krch->mod;
        vnode->node = node;
        
        kr_skiplist_insert(krch->vnode_list, vnode->hash, vnode);
    }
    node->status = KR_NODE_STATUS_INSERTED;
    krch->node_num++;
    
    return 0;
}


int kr_conhash_delete(T_KRConHash *krch, T_KRActualNode *node)
{
    int i;
    T_KRVirtualNode *vnode = NULL;
    
    for (i = 0; i < node->weights; i++) {
        vnode = &node->vnodes[i];
        
        kr_skiplist_delete(krch->vnode_list, vnode->hash, vnode);
    }
    node->status = KR_NODE_STATUS_DELETED;
    krch->node_num--;
    
    return 0;   
}


T_KRActualNode *kr_conhash_locate(T_KRConHash *krch, char *object)
{
    T_KRVirtualNode *vnode = NULL;
    unsigned int hash = krch->hash_func(object) % krch->mod;
    
    vnode = kr_skiplist_lookup(krch->vnode_list, hash);
    return vnode->node;
}
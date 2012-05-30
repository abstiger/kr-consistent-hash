#include "kr_conhash.h"
#include <stdint.h>
#include <limits.h>

static T_KRActualNode *kr_conhash_create_node(char *id, unsigned int weights);
static void kr_conhash_free_node(T_KRActualNode *node);
static int kr_conhash_insert_node(T_KRConHash *krch, T_KRActualNode *node);
static int kr_conhash_delete_node(T_KRConHash *krch, T_KRActualNode *node);

/*crc16tab*/
static const uint16_t crc16tab[256]= {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

/* the default hash function:crc16 */
static inline unsigned int kr_crc16_hash(const void *buf)
{
    char *str = (char *)buf;
    int i, len = strlen(str);
    uint16_t crc = 0;
    for (i = 0; i < len; i++) {
        crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *str++)&0x00FF];
    }
    
    return (unsigned int)crc;
}

/* the default vnode compare function */
static inline int kr_vnode_compare(const void * v1, const void * v2)
{
    T_KRVirtualNode *vnode1 = (T_KRVirtualNode *)v1;
    T_KRVirtualNode *vnode2 = (T_KRVirtualNode *)v2;
  
    return strcmp(vnode1->vid, vnode2->vid);
}

/* the default node compare function */
static inline int kr_node_compare(const void * v1, const void * v2)
{  
    T_KRActualNode *node1 = (T_KRActualNode *)v1;
    T_KRActualNode *node2 = (T_KRActualNode *)v2;
    
    return strcmp(node1->id, node2->id);
}


T_KRConHash *
kr_conhash_construct(KRHashFunc hash_func)
{
    T_KRConHash *krch = malloc(sizeof(T_KRConHash));
    if (krch == NULL) {
        fprintf(stderr, "malloc krch failed!\n");
        return NULL;
    }
    
    krch->hash_func = hash_func?hash_func:kr_crc16_hash;
    krch->vnode_list = kr_skiplist_create(kr_vnode_compare);
    if (krch->vnode_list == NULL) {
        fprintf(stderr, "kr_skiplist_create vnode list failed!\n");
        return NULL;
    }
    krch->node_list = kr_skiplist_create(kr_node_compare);
    if (krch->node_list == NULL) {
        fprintf(stderr, "kr_skiplist_create node list failed!\n");
        return NULL;
    }
    
    return krch;
}


static inline void _kr_free_node_foreach(void *value, void *data)
{
    T_KRActualNode *node = (T_KRActualNode *)value;
    kr_conhash_free_node(node);
}


void kr_conhash_destruct(T_KRConHash *krch)
{
    kr_skiplist_foreach(krch->node_list, _kr_free_node_foreach, NULL);
    kr_skiplist_free(krch->vnode_list);
    kr_skiplist_free(krch->node_list);
    free(krch);
}


static T_KRActualNode *kr_conhash_create_node(char *id, unsigned int weights)
{
    T_KRActualNode *node = malloc(sizeof(T_KRActualNode));
    if (node == NULL) {
        fprintf(stderr, "malloc node failed!\n");
        return NULL;
    }
    
    strncpy(node->id, id, sizeof(node->id));
    node->weights = weights;
    node->kicks = 0;
    node->vnodes = calloc(weights, sizeof(T_KRVirtualNode));
    if (node->vnodes == NULL) {
        fprintf(stderr, "malloc node->vnodes failed!\n");
        return NULL;
    }
    
    node->status = KR_NODE_STATUS_CREATED;
    return node;
}


static void kr_conhash_free_node(T_KRActualNode *node)
{
    free(node->vnodes);
    free(node);
}


static int kr_conhash_insert_node(T_KRConHash *krch, T_KRActualNode *node)
{
    int i;
    T_KRVirtualNode *vnode = NULL;
    
    for (i = 0; i < node->weights; i++) {
        vnode = &node->vnodes[i];
        
        snprintf(vnode->vid, sizeof(vnode->vid), "%s-%03d", node->id, i);
        vnode->hash = krch->hash_func(vnode->vid);
        vnode->node = node;
        
        kr_skiplist_insert(krch->vnode_list, vnode->hash, vnode);
    }
    node->hash = krch->hash_func(node->id);
    kr_skiplist_insert(krch->node_list, node->hash, node);
    
    node->status = KR_NODE_STATUS_INSERTED;
    return 0;
}


static int kr_conhash_delete_node(T_KRConHash *krch, T_KRActualNode *node)
{
    int i;
    T_KRVirtualNode *vnode = NULL;
    
    for (i = 0; i < node->weights; i++) {
        vnode = &node->vnodes[i];
        
        kr_skiplist_delete(krch->vnode_list, vnode->hash, vnode);
    }
    kr_skiplist_delete(krch->node_list, node->hash, node);
    
    node->status = KR_NODE_STATUS_DELETED;
    return 0;
}


T_KRActualNode *kr_conhash_lookup(T_KRConHash *krch, char *id)
{
    T_KRActualNode *ret_node;
    T_KRActualNode stNode = {0};
    T_KRActualNode *node = &stNode;
    
    node->hash = krch->hash_func(id);
    strncpy(node->id, id, sizeof(node->id));
    ret_node = kr_skiplist_lookup(krch->node_list, node->hash, node);
    
    return ret_node;
}


int kr_conhash_add(T_KRConHash *krch, char *id, unsigned int weights)
{
    T_KRActualNode *node = NULL;
    node = kr_conhash_create_node(id, weights);
    if (node == NULL) {
        fprintf(stderr, "kr_conhash_create_node failed\n");
        return -1;
    }
    
    kr_conhash_insert_node(krch, node);
    return 0;
}


int kr_conhash_remove(T_KRConHash *krch, char *id)
{
    T_KRActualNode *node = NULL;
    node = kr_conhash_lookup(krch, id);
    if (node == NULL) {
        fprintf(stderr, "id[%s] not found\n", id);
        return -1;
    }
    
    kr_conhash_delete_node(krch, node);
    kr_conhash_free_node(node);
    return 0;
}


int kr_conhash_adjust_weights(T_KRConHash *krch, char *id, unsigned int weights)
{
    if (kr_conhash_remove(krch, id) != 0) {
        fprintf(stderr, "kr_conhash_remove [%s] failed\n", id);
        return -1;
    }
    
    if (kr_conhash_add(krch, id, weights) != 0) {
        fprintf(stderr, "kr_conhash_add [%s] [%u] failed\n", id, weights);
        return -1;
    }
    return 0;
}


T_KRActualNode *kr_conhash_locate(T_KRConHash *krch, char *object)
{
    T_KRVirtualNode *vnode = NULL;
    unsigned int hash = krch->hash_func(object);
    
    vnode = kr_skiplist_lookup_cycle(krch->vnode_list, hash);
    
    vnode->node->kicks = ++vnode->node->kicks % UINT_MAX;
    return vnode->node;
}
#include "kr_conhash.h"
#include <stdint.h>
#include <assert.h>

static char *machines[5] = {"tiger-laptop", 
                            "rabbit-laptop", 
                            "dragon-laptop", 
                            "snake-laptop", 
                            "horse-laptop"};


void kr_vnode_print(void *value, void *data) 
{
    T_KRVirtualNode *vnode = (T_KRVirtualNode *)value;
    printf("vnode->hash[%u], vnode->vid[%s], node->id[%s]\n", 
           vnode->hash, vnode->vid, vnode->node->id);
}


void kr_node_print(void *value, void *data) 
{
    T_KRActualNode *node = (T_KRActualNode *)value;
    printf("node->hash[%u], node->id[%s], node->weights[%u], node->kicks[%u]\n", 
           node->hash, node->id, node->weights, node->kicks);
}


int main(int argc, char *argv[])
{
    int i;
    int result = 0;
    T_KRConHash *krch = NULL;
    T_KRActualNode *kick_node = NULL;
    char kick_value[200] = {0};
    
    krch = kr_conhash_construct(NULL);
    assert(krch);
    result = kr_conhash_add(krch, machines[0], 9);
    assert(result == 0);
    result = kr_conhash_add(krch, machines[1], 3);
    assert(result == 0);
    result = kr_conhash_add(krch, machines[2], 12);
    assert(result == 0);
    result = kr_conhash_add(krch, machines[3], 4);
    assert(result == 0);
    result = kr_conhash_add(krch, machines[4], 8);
    assert(result == 0);
    
printf("node_num[%u], vnode_num[%u]\n", \
           krch->node_list->length, krch->vnode_list->length);
    
    for (i = 0; i < 20; i++) {
        if (i%3 == 0) {
            snprintf(kick_value, sizeof(kick_value), "haha-%03d", i);
        } else if (i%3 == 1) {
            snprintf(kick_value, sizeof(kick_value), "hehe-%03d", i);
        } else {
            snprintf(kick_value, sizeof(kick_value), "hihi-%03d", i);
        }
        kick_node = kr_conhash_locate(krch, kick_value);
        printf("kick [%s] node is[%s]\n", kick_value, kick_node->id);
    }
    
    kr_skiplist_foreach(krch->vnode_list, kr_vnode_print, NULL);
    kr_skiplist_foreach(krch->node_list, kr_node_print, NULL);
    
    result = kr_conhash_remove(krch, machines[0]);
    assert(result == 0);
    printf("delete machine [%s]\n", machines[0]);
    result = kr_conhash_remove(krch, machines[4]);
    assert(result == 0);
    printf("delete machine [%s]\n", machines[4]);
    result = kr_conhash_adjust_weights(krch, machines[1], 5);
    assert(result == 0);
    printf("adjust machine[%s] weights[%u]\n", machines[1], 5);
printf("node_num[%u], vnode_num[%u]\n", \
           krch->node_list->length, krch->vnode_list->length);
    
    for (i = 0; i < 20; i++) {
        if (i%3 == 0) {
            snprintf(kick_value, sizeof(kick_value), "haha-%03d", i);
        } else if (i%3 == 1) {
            snprintf(kick_value, sizeof(kick_value), "hehe-%03d", i);
        } else {
            snprintf(kick_value, sizeof(kick_value), "hihi-%03d", i);
        }
        kick_node = kr_conhash_locate(krch, kick_value);
        printf("kick [%s] node is[%s]\n", kick_value, kick_node->id);
    }
    
    kr_skiplist_foreach(krch->vnode_list, kr_vnode_print, NULL);
    kr_skiplist_foreach(krch->node_list, kr_node_print, NULL);

    kr_conhash_destruct(krch);
    
    printf("Sucess!\n");
    return 0;
}



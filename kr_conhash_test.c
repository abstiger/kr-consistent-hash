#include "kr_conhash.h"

void kr_skiplist_print(T_KRSkipList *krsl) 
{
    T_KRSkipListNode *node = krsl->header->forward[0], *next;

    while(node) {
        next = node->forward[0];
        T_KRVirtualNode *vnode = node->value;
        printf("node->key[%u], vnode->hash[%u], vnode->vid[%s], vnode->node->id[%s]\n", 
                node->key, vnode->hash, vnode->vid, vnode->node->id);
        
        node = next;
    }
}

int main(int argc, char *argv[])
{
    int i;
    T_KRConHash *krch;
    krch = kr_conhash_construct(13, NULL);
    
    T_KRActualNode *node[5];
    node[0] = kr_conhash_create_node("tiger-laptop", 9);
    node[1] = kr_conhash_create_node("rabbit-laptop", 1);
    node[2] = kr_conhash_create_node("dragon-laptop", 2);
    node[3] = kr_conhash_create_node("snake-laptop", 4);
    node[4] = kr_conhash_create_node("horse-laptop", 8);
    
    
    kr_conhash_insert(krch, node[0]);
    kr_conhash_insert(krch, node[1]);
    kr_conhash_insert(krch, node[2]);
    kr_conhash_insert(krch, node[3]);
    kr_conhash_insert(krch, node[4]);

printf("node_num[%u], vnode_num[%u]\n", krch->node_num, krch->vnode_list->length);
    
    T_KRActualNode *kick_node;
    char kick_value[200] = {0};
    for (i = 0; i < 20; i++) {
        snprintf(kick_value, sizeof(kick_value), "haha-hehe-hihi-%03d", i);
        kick_node = kr_conhash_locate(krch, kick_value);
        printf("kick [%s] node is[%s]\n", kick_value, kick_node->id);
    }
    
    kr_skiplist_print(krch->vnode_list);
    
    kr_conhash_delete(krch, node[0]);
    printf("delete node [%s]\n", node[0]->id);
    kr_conhash_delete(krch, node[4]);
    printf("delete node [%s]\n", node[4]->id);
    
    kr_skiplist_print(krch->vnode_list);
printf("node_num[%u], vnode_num[%u]\n", krch->node_num, krch->vnode_list->length);

    for (i = 0; i < 20; i++) {
        snprintf(kick_value, sizeof(kick_value), "haha-hehe-hihi-%03d", i);
        kick_node = kr_conhash_locate(krch, kick_value);
        printf("kick [%s] node is[%s]\n", kick_value, kick_node->id);
    }
    
    
    kr_conhash_free_node(node[0]);
    kr_conhash_free_node(node[1]);
    kr_conhash_free_node(node[2]);
    kr_conhash_free_node(node[3]);
    kr_conhash_free_node(node[4]);

printf("node_num[%u], vnode_num[%u]\n", krch->node_num, krch->vnode_list->length);

    kr_conhash_destruct(krch);
    
    return 0;
}



#include "kr_skiplist.h"

#define VALUE_LENGTH 20


static int MYCompare(const void *v1, const void *v2)
{
    return *(int *)v1 - *(int *)v2;
}


int main(int argc, char *argv[])
{
    printf("sizeof(T_KRSkipList)=[%d]\n", sizeof(T_KRSkipList));
    printf("sizeof(T_KRSkipListNode)=[%d]\n", sizeof(T_KRSkipListNode));
    
    unsigned int value[VALUE_LENGTH] = {0};
    int i;
    
    T_KRSkipList *krsl;
    krsl = kr_skiplist_create(MYCompare);
    
    for (i=0; i<VALUE_LENGTH; ++i) {
        value[i] = i+20;
        if (i == 6) value[i] = value[i-1];
        printf("value[%d]=[%d]\n", i, value[i]);
        kr_skiplist_insert(krsl, value[i], &value[i]);
    }
    
    void *pValue;
    pValue = kr_skiplist_lookup_cycle(krsl, value[7]);
    if (*(int *)pValue == value[7]) {
        printf("contain value[%d]:[%d]\n", 7, value[7]);
    } else {
        printf("not contain value[%d]:[%d], [%d]\n", 
               7, value[7], *(int *)pValue);
    }
     
    int nRet = kr_skiplist_delete(krsl, value[7], &value[7]);
    printf("kr_skiplist_delete value[%d]:[%d] [%d]\n", 7, value[7], nRet);
    
    pValue = kr_skiplist_lookup_cycle(krsl, value[7]);
    if (*(int *)pValue == value[7]) {
        printf("contain value[%d]:[%d]\n", 7, value[7]);
    } else {
        printf("not contain value[%d]:[%d], [%d]\n", 
               7, value[7], *(int *)pValue);
    }

printf("level[%d], length[%d]\n", krsl->level, krsl->length);

    kr_skiplist_free(krsl);
    
    printf("Sucess!\n");
    return 0;
}
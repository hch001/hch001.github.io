#include "skiplist.h"
#include <stdio.h>
#include <mm_malloc.h>
#include <memory.h>

int compareRobjs(robj* obj1,robj* obj2){
    if(obj1==obj2) return 0;

    int i = 0;
    while(i < SKIPLIST_MAXOBJSIZE && (obj1[i]*obj2[i]==0) && obj1[i]==obj2[i])
        i++;

    return (i==SKIPLIST_MAXOBJSIZE) ? 0 : obj1[i]-obj2[i];
}

int getRandomLevel(){
    int res=1;
    while(rand()%2&&res<SKIPLIST_MAXLEVEL)
        res++;

    return res;
}

skiplistNode* sl_createNode(int level,double score,robj* obj){
    skiplistNode* node = (skiplistNode*)malloc(sizeof(*node)+level*sizeof(struct skiplistLevel));

    node->score = score;
    node->backward = NULL;
    node->obj = obj;

    return node;
}

skiplist* sl_createList(){
    skiplist* list = malloc(sizeof(*list));
    list->length = 0;
    list->level = 1;

    list->header = sl_createNode(SKIPLIST_MAXLEVEL,0,NULL);
    list->header->backward = NULL;
    list->tail = NULL;

    for(int i=0;i<SKIPLIST_MAXLEVEL;i++){
        list->header->level[i].forward = NULL;
//        list->header->level[i].span = 0;
    }

    return list;
}

skiplistNode* sl_insert(skiplist* list,double score,robj* obj){

    skiplistNode *x,*update[SKIPLIST_MAXLEVEL];

    unsigned int rank[SKIPLIST_MAXLEVEL];

    // 随机生成的level
    int i,level;

    x = list->header;

    for(i = list->level-1; i >= 0 ;i--){
        rank[i] = (i==SKIPLIST_MAXLEVEL-1) ? 0 :rank[i+1];

        while(x->level[i].forward &&
              (x->level[i].forward->score < score || (x->level[i].forward->score == score && compareRobjs(obj,x->level[i].forward->obj)>0)) ){
            rank[i] += x->level[i].span; // 这一步注意是加的x的span 而不是forward的
            x = x->level[i].forward;
        }

        update[i] = x;
    }

    level = getRandomLevel();
    x = sl_createNode(level,score,obj);

    // level到 SKIPLIST_MAXLEVEL之间的层数需要初始化为0
    if(level > list->level){
        for(i = list->level; i < level; i++){
            rank[i] = 0;
            update[i] = list->header;
            update[i]->level[i].span = list->length;
        }
        list->level = level;
    }

    // 插入节点
    for(i = 0;i<level;i++){
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;

        // 设置span
        x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
        update[i]->level[i].span = rank[0] - rank[i] + 1;
    }

    // 未涉及到的节点span
    for(i = level; i<list->level;i++){
        update[i]->level[i].span++;
    }

    // backward仅用于反向遍历，如果不需要反向遍历,去掉下面一行也行
    x->backward = (update[0]==list->header) ? NULL : update[0];

    if(x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        list->tail = x;

    list->length++;

    return x;
}

skiplistNode* sl_insertWithoutSpan(skiplist* list, double score,robj* obj){
    skiplistNode* x = list->header;

    skiplistNode* update[SKIPLIST_MAXLEVEL];
    int i;
    for(i = list->level-1; i>=0 ; i--){
        while(x->level[i].forward &&
              (x->level[i].forward->score < score || (x->level[i].forward->score == score && (compareRobjs(obj,x->level[i].forward->obj)>0)))){
            x = x->level[i].forward;
        }

        update[i] = x;
    }

    int level = getRandomLevel();
    x = sl_createNode(level,score,obj);

    if(level > list->level){
        for(i = list->level; i <level; i++){
            update[i] = list->header;
        }
        list->level = level;
    }

    for(i = 0; i < level ;i++){
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;
    }

    x->backward = (update[0]==list->header) ? NULL : update[0];

    if(x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        list->tail = x;

    list->length++;

    return x;
}

bool sl_deleteNodeByScoreAndObj(skiplist* list,double score,robj* obj){
    skiplistNode *x,*update[SKIPLIST_MAXLEVEL];

    int i;
    x = list->header;

    for(i = list->level-1; i >= 0; i--){
        while(x->level[i].forward &&
                (x->level[i].forward->score < score || (x->level[i].forward->score == score && compareRobjs(obj,x->level[i].forward->obj)>0))){
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    // 若遍历之后没有找到，说明不存在
    skiplistNode* target = update[0]->level[0].forward;
    if(target->score!=score || compareRobjs(target->obj,obj)!=0)
        return false;

    // 改变指针
    for(i = 0; i < list->level; i++){
        // 仅处理需要变动的层
        if(update[i]->level[i].forward){
            update[i]->level[i].forward = update[i]->level[i].forward->level[i].forward;
        }
    }

    // 回收内存
    free(target);

    return true;
}

bool sl_deleteNode(skiplist* list,skiplistNode* node){
    return sl_deleteNodeByScoreAndObj(list,node->score,node->obj);
}

skiplistNode* sl_search(skiplist* list, double score, robj* obj){
    skiplistNode* x = list->header;

    int i;
    for(i = SKIPLIST_MAXLEVEL-1; i >= 0;i--){
        while(x->level[i].forward && (x->level[i].forward->score < score || (x->level[i].forward->score == score && compareRobjs(obj,x->level[i].forward->obj)>0))){
            x = x->level[i].forward;
        }
        if(x->level[i].forward && compareRobjs(obj,x->level[i].forward->obj)==0)
            return x->level[i].forward;
    }

    return NULL;
}

bool sl_Traverse(skiplist* list){
    if(list==NULL || list->header == NULL) return false;
    skiplistNode* node = list->header->level[0].forward;

    while(node){
        printf("%s ",node->obj);
        node = node->level[0].forward;
    }
    return true;
}

bool sl_RTraverse(skiplist* list){
    if(list==NULL) return false;

    skiplistNode* node = list->tail;
    while(node){
        printf("%s ",node->obj);
        node = node->backward;
    }

    return true;
}

int main() {

    char s1[] = {'a','b','c','\0'};
    char s2[] = {'d','e','f','\0'};
    char s3[] = {'g','h','i','\0'};

    skiplist* list = sl_createList();
    sl_insertWithoutSpan(list,1,s1);
    sl_insertWithoutSpan(list,2,s2);
    sl_insertWithoutSpan(list,3,s3);

//    skiplistNode* res = sl_search(list,3,"ghi");

//    printf("res:%s\n",res->obj);
    sl_deleteNodeByScoreAndObj(list,3,"ghi");
    sl_Traverse(list);
//    sl_RTraverse(list);

    return 0;
}

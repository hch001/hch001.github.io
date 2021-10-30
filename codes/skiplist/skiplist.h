#ifndef __SKIPLIST_H
#define __SKIPLIST_H

#define false 0
#define true 1
#define SKIPLIST_MAXLEVEL 8
#define SKIPLIST_MAXOBJSIZE 128

typedef char robj;
typedef int bool;

typedef struct skiplistNode {
    robj* obj;
    double score;
    struct skiplistNode *backward; // 上一个节点

    struct skiplistLevel{
        struct skiplistNode* forward; // 下一个节点
        unsigned int span; // 跨度，将所有的span相加就可以得到该节点在跳表中的位置
    }level[];
}skiplistNode;

typedef struct skiplist {
    struct skiplistNode *header, *tail;
    unsigned long length;
    int level;
} skiplist;

int compareRobjs(robj* obj1,robj* obj2);
int getRandomLevel();
skiplistNode* sl_createNode(int level,double score,robj* obj);
skiplist* sl_createList();
skiplistNode* sl_insert(skiplist* list,double score,robj* obj);
skiplistNode* sl_insertWithoutSpan(skiplist* list, double score,robj* obj);
skiplistNode* sl_search(skiplist* list, double score, robj* obj);
bool sl_deleteNodeByScoreAndObj(skiplist* list,double score,robj* obj);
bool sl_deleteNode(skiplist* list,skiplistNode* node);
bool sl_Traverse(skiplist* list);
bool sl_RTraverse(skiplist* list);

#endif
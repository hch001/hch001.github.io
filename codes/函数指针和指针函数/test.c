#include <stdio.h>
#include <mm_malloc.h>

int func0(int a,int b){
    return 1;
}

int* func1(int a,int b){
    return NULL;
}

/* 返回指向长度为20的数组的指针的函数 */
int (*func2(void))[20]{

    int (*res)[20];
    res = malloc(sizeof(int)*20);
    (*res)[0]=149;

    return res;
}

/* func_type 是一个函数指针 */
typedef int(*func_type)(int);

/* 返回一个指向长度为10数组(其中每个元素都是函数指针(接收参数为int，返回值为int))的指针的函数 */
func_type (*func3(void))[10]{
    return NULL;
}



struct file_ops{
    size_t (*op_read)(FILE*, void* ,size_t, size_t);
    size_t (*op_write)(FILE*, void* ,size_t, size_t);
    size_t (*op_close)(FILE*);
};

int main(){

    int (*f0)(int,int);
    f0 = func0;

    int* (*f1)(int,int);
    f1 = func1;

    int (*(*f2)(void))[20];
    f2 = func2;

    func_type (*(*f3)(void))[10];
    f3 = func3;

    return 0;
}

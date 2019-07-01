//
//  main.c
//  b+tree
//
//  Created by 李寒雪 on 2019/4/28.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include "BplusTree.h"
#define data_num   (10000 * 5000)
#define trash_num  (10000 * 5000 - 10)
int main(int argc, const char * argv[]) {
    //time_t start,end;
    int *a=NULL;
    int *b=NULL;
    a = (int*) malloc(sizeof(int)*data_num);
    b = (int*) malloc(sizeof(int)*trash_num);
    if(a==NULL){
        printf("内存分配失败\n");
    }
    if(b==NULL){
        printf("内存分配失败\n");
    }
    for(int i=0;i<data_num;++i){
        a[i]=i;
        //printf("%d\t",a[i]);
    }
    printf("\n");
    for(int i=0;i<trash_num;++i){
        b[i]=i;
        //printf("%d\t",b[i]);
    }
    srand((unsigned int)(time(NULL)));
    for(int i=0;i<data_num;++i){
        
        int k=rand()%data_num;
        int temp=a[i];
        a[i]=a[k];
        a[k]=temp;
    }
   
    for(int i=0;i<trash_num;++i){
        int k=rand()%trash_num;
        int temp=b[i];
        b[i]=b[k];
        b[k]=temp;
    }

    
    
    
    clock_t start_time, end_time;
    printf("创建树：");
    start_time = clock();
    node* pr = CreatBplusTree(a, data_num);
    end_time = clock();
    printf("创建结束\n");
    printf("创建时间是：%f\n",(double)(end_time - start_time) / CLOCKS_PER_SEC);
    
    printf("删除树：");
    start_time =clock();
    pr = BplusTreeTrash(pr, b,trash_num);
    end_time = clock();
    printf("删除结束\n");
    printf("删除时间是：%f\n",(double)(end_time - start_time) / CLOCKS_PER_SEC);
    
    //FILE *fp=fopen("text.txt", "a");
    //fputs("jiji\nlili", fp);
    //fputs("hh\tkk", fp);
    //fclose(fp);
    printf("删除后树为：\n");
    BplusTreeLeveOrder(pr);
    free(a);
    free(b);
    printf("stop\n");
    return 0;
}

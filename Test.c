//
//  Test.c
//  b+tree
//
//  Created by 李寒雪 on 2019/5/13.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include <stdio.h>
#include "BplusTree.h"

void PrintBplusTreeNode(node *temp_pr,FILE *fp){
    char key_str[100];
    for(int i=0;i<temp_pr->key_node_num;++i){
        //printf("%d ",temp_pr->after_first_childnode_pr[i].key);
        sprintf(key_str, "%d",temp_pr->after_first_childnode_pr[i].key);
        fputs(key_str, fp);
        fputs(" ", fp);
    }
    //printf("\t");
    fputs("\t", fp);
    //printf("关键值个数：%d\t",temp_pr->key_node_num);
}


void BplusTreeLeveOrder(node *root_node_pr){
    FILE *fp;
    fp=fopen("leve_order.txt", "a");
    //if(fp==NULL){
    //    printf("打不开文件\n");
    //}
    if(root_node_pr==NULL){
        printf("树为空");
    }
    else{
        leve_order_node *que_front_pr=(leve_order_node*)malloc(sizeof(leve_order_node));
        que_front_pr->node_pr= root_node_pr;
        que_front_pr->deepth_flag=1;
        que_front_pr->next_node_pr=NULL;
        leve_order_node *que_rear_pr=que_front_pr;
        do {
            PrintBplusTreeNode(que_front_pr->node_pr,fp);
            if(que_front_pr->next_node_pr!=NULL && que_front_pr->deepth_flag!=que_front_pr->next_node_pr->deepth_flag){
                //printf("  深度为%d\n",que_front_pr->deepth_flag);
                fputs("\n", fp);
            }
            else if(que_front_pr->next_node_pr==NULL){
                //printf("  深度为%d\n",que_front_pr->deepth_flag);
                fputs("\n", fp);
            }
            if(que_front_pr->node_pr->state==inside_node){
                que_rear_pr->next_node_pr=(leve_order_node*)malloc(sizeof(leve_order_node));
                que_rear_pr->next_node_pr->node_pr=que_front_pr->node_pr->first_childnode_pr;
                que_rear_pr->next_node_pr->deepth_flag=que_front_pr->deepth_flag+1;
                que_rear_pr->next_node_pr->next_node_pr=NULL;
                que_rear_pr=que_rear_pr->next_node_pr;
                for(int i=0;i<=que_front_pr->node_pr->key_node_num-1;++i){
                    que_rear_pr->next_node_pr=(leve_order_node*)malloc(sizeof(leve_order_node));
                    que_rear_pr->next_node_pr->node_pr = que_front_pr->node_pr->after_first_childnode_pr[i].childnode_pr;
                    que_rear_pr->next_node_pr->deepth_flag=que_front_pr->deepth_flag+1;
                    que_rear_pr->next_node_pr->next_node_pr=NULL;
                    que_rear_pr=que_rear_pr->next_node_pr;
                }
            }
            leve_order_node *temp_que_front_pr=que_front_pr;
            que_front_pr=que_front_pr->next_node_pr;
            free(temp_que_front_pr);
        }while (que_front_pr!=NULL);
    }
    fclose(fp);
}


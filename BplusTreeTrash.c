//
//  BplusTreeTrash.c
//  b+tree
//
//  Created by 李寒雪 on 2019/5/14.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include <stdio.h>
#include "BplusTree.h"
node* BplusTreeTrash(node *root_node_pr,int data[],int data_num){
    if(data_num==0){
        printf("没有要要求删除的数据\n");
        return NULL;
    }
    for(int i =0;i<data_num;++i){
        BplusTreeDelete(data[i], root_node_pr);
        if(root_node_pr->key_node_num==0&&root_node_pr->state==inside_node){
            node *pr_free=root_node_pr;
            root_node_pr=root_node_pr->first_childnode_pr;
            free(pr_free);
        }
        else if (root_node_pr->key_node_num==0&&root_node_pr->state==leaf_node){
            free(root_node_pr);
            root_node_pr=NULL;
            printf("当前树为空，已经删除完\n");
        }
        //if(i==data_num-1){
        //    printf("删除%d之后：\n",data[i]);
        //    TestBplusTree(root_node_pr);
        //}
        //printf("删除%d之后：\n",data[i]);
        // if(i==data_num-1){
        //     BplusTreeLeveOrder(root_node_pr);
        // }
        //printf("\n");
        
    }
    //TestBplusTree(root_node_pr);
    return root_node_pr;
}

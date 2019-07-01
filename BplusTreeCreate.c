//
//  BplusTreeCreate.c
//  b+tree
//
//  Created by 李寒雪 on 2019/5/13.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include <stdio.h>
#include "BplusTree.h"
/*********B+树的创建*********/
node* CreatBplusTree(int data[],int data_num){
    if(data_num==0){
        printf("没有数据建立一颗B+树");
        return NULL;
    }
    node *root_node_pr = (node*) malloc(sizeof(node));
    InitNode(root_node_pr);
    root_node_pr->state=leaf_node;
    for(int i=0;i<data_num;i++){
        int temp=data[i];
        split_state split_state = BplusTreeInsertData(temp, root_node_pr);
        if (split_state.is_split==yes){
            node *new_root_node_pr = (node*)malloc(sizeof(node));
            new_root_node_pr->state=inside_node;
            new_root_node_pr->next_leafnode_pr=NULL;
            new_root_node_pr->key_node_num=1;
            new_root_node_pr->first_childnode_pr=root_node_pr;
            new_root_node_pr->after_first_childnode_pr[0].childnode_pr=split_state.new_node_pr;
            new_root_node_pr->after_first_childnode_pr[0].key=split_state.up_data;
            root_node_pr=new_root_node_pr;
        }
        //if(i==data_num-1){
         //   printf("第%d个数据%d放入后：\n",i+1,temp);
            //TestBplusTree(root_node_pr);

        //}
        //if(i==data_num-1){
        //    BplusTreeLeveOrder(root_node_pr);
        //}
        //printf("\n");
    }
    return root_node_pr;
}


//
//  BplusTreeSearch.c
//  b+tree
//
//  Created by 李寒雪 on 2019/5/20.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include <stdio.h>
#include "BplusTree.h"
search_return_state BplusTreeSearch(int data,node *root_node_pr){
    search_return_state search_return_state;
    if(root_node_pr->key_node_num==0){
        printf("树为空，没可以查找的值\n");
        search_return_state.search_success=lose;
        search_return_state.search_num=0;
        return search_return_state;
    }
    node *temp_node_pr=root_node_pr;
    while (temp_node_pr->state==inside_node) {
        search_return_state = SearchKey(data, temp_node_pr);
        if(search_return_state.search_num==-1){
            temp_node_pr=temp_node_pr->first_childnode_pr;
        }
        else{
            temp_node_pr=temp_node_pr->after_first_childnode_pr[search_return_state.search_num].childnode_pr;
        }
    }
    search_return_state =SearchKey(data, temp_node_pr);
    if(search_return_state.search_success==lose){
        printf("查找失败，没有数据%d\n",data);
        return search_return_state;
    }
    else{
        printf("查找%d成功，其所在叶子结点为：\n",data);
        //PrintBplusTreeNode(temp_node_pr);
        return search_return_state;
    }
    //return search_return_state;
}

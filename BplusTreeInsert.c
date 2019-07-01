//
//  BplusTreeCreate.c
//  b+tree
//
//  Created by 李寒雪 on 2019/5/13.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include <stdio.h>
#include "BplusTree.h"
/**********B+树关键值插入******/
split_state BplusTreeInsertData(int data,node *temp_node_pr){
    split_state split_state;
    if(temp_node_pr->state==inside_node){
        search_return_state search_return=SearchKey(data, temp_node_pr);
        if(search_return.search_num==-1){
            split_state = BplusTreeInsertData(data, temp_node_pr->first_childnode_pr);
            if(split_state.is_split==yes){
                for(int i=temp_node_pr->key_node_num-1;i>=0;--i){
                    temp_node_pr->after_first_childnode_pr[i+1]=temp_node_pr->after_first_childnode_pr[i];
                    temp_node_pr->after_first_childnode_pr[i+1].childnode_pr=temp_node_pr->after_first_childnode_pr[i].childnode_pr;
                }
                temp_node_pr->after_first_childnode_pr[0].key=split_state.up_data;
                temp_node_pr->after_first_childnode_pr[0].childnode_pr=split_state.new_node_pr;
                temp_node_pr->key_node_num=temp_node_pr->key_node_num+1;
                if(temp_node_pr->key_node_num>=degree){
                    split_state = BplusTreeSplit(temp_node_pr);
                    return split_state;//内部结点的分裂
                }
            }
            split_state.is_split=no;
            split_state.new_node_pr=NULL;
            split_state.up_data=0;
            return split_state;
        }//内部结点继续在第一个孩子指针中查找
        else{
            split_state = BplusTreeInsertData(data, temp_node_pr->after_first_childnode_pr[search_return.search_num].childnode_pr);
            int i;
            if(split_state.is_split==yes){
                for(i=temp_node_pr->key_node_num-1;i>=search_return.search_num+1;--i){
                    temp_node_pr->after_first_childnode_pr[i+1]=temp_node_pr->after_first_childnode_pr[i];
                    temp_node_pr->after_first_childnode_pr[i+1].childnode_pr=temp_node_pr->after_first_childnode_pr[i].childnode_pr;
                }
                temp_node_pr->after_first_childnode_pr[i+1].key=split_state.up_data;
                temp_node_pr->after_first_childnode_pr[i+1].childnode_pr=split_state.new_node_pr;
                temp_node_pr->key_node_num=temp_node_pr->key_node_num+1;
                if(temp_node_pr->key_node_num>=degree){
                    split_state = BplusTreeSplit(temp_node_pr);
                    return split_state;//内部结点的分类
                }
            }
            split_state.is_split=no;
            split_state.new_node_pr=NULL;
            split_state.up_data=0;
            return split_state;
        }//内部结点继续在后面孩子指针中查找
    
}//对于插入点是内部结点
    else{
        search_return_state search_return=SearchKey(data, temp_node_pr);
        if (search_return.search_success==1){
            printf("树中已经有该数据，存在重复数据插入");
            split_state.is_split=no;
            split_state.new_node_pr=NULL;
            split_state.up_data=0;
            return split_state;
        }
        else{
            int i;
            for(i=temp_node_pr->key_node_num-1;i>=search_return.search_num+1;--i){
                temp_node_pr->after_first_childnode_pr[i+1]=temp_node_pr->after_first_childnode_pr[i];
            }
            temp_node_pr->after_first_childnode_pr[i+1].childnode_pr=NULL;
            temp_node_pr->after_first_childnode_pr[i+1].key=data;
            temp_node_pr->key_node_num=temp_node_pr->key_node_num+1;
            if(temp_node_pr->key_node_num>=degree){
                split_state = BplusTreeSplit(temp_node_pr);
                return split_state;//叶子结点的分裂
            }
        }
    }
    split_state.is_split=no;
    split_state.new_node_pr=NULL;
    split_state.up_data=0;
    return split_state;//对于插入点是叶子结点
}

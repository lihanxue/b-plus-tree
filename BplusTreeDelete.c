//
//  BplusTreeDelete.c
//  b+tree
//
//  Created by 李寒雪 on 2019/5/14.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include <stdio.h>
#include "BplusTree.h"

merge_state BplusTreeDelete(int data,node *temp_node_pr){
    merge_state merge_state;
    if(temp_node_pr->state==inside_node){
        search_return_state search_return=SearchKey(data, temp_node_pr);
        if(search_return.search_num==-1){
            merge_state = BplusTreeDelete(data, temp_node_pr->first_childnode_pr);
            if(merge_state.is_unfull==unfull){
                borrow_state borrow_state = BplusTreeBorrow(temp_node_pr, -1);
                if(borrow_state.can_barrow==can){
                    merge_state.is_unfull=full;
                    return merge_state;
                }//第一个孩子结点借了，当前结点返回满
                else{
                    BplusTreeMerge(temp_node_pr, search_return.search_num);//合并两个孩子结点
                    if(temp_node_pr->key_node_num<((int)(ceil(degree/2)))){
                        merge_state.is_unfull=unfull;
                        return merge_state;
                    }//若当前节点合并操作之后不满，返回不满
                }//第一个孩子没有得借
            }//第一个孩子结点不满了，开始做借和合并操作
            merge_state.is_unfull=full;
            return merge_state;
        }//内部结点在第一个孩子结点中去找
        else{
            merge_state = BplusTreeDelete(data, temp_node_pr->after_first_childnode_pr[search_return.search_num].childnode_pr);
            if(merge_state.is_unfull==unfull){
                borrow_state borrow_state = BplusTreeBorrow(temp_node_pr, search_return.search_num);
                if(borrow_state.can_barrow==can){
                    merge_state.is_unfull=full;
                    return merge_state;
                }
                else{
                    BplusTreeMerge(temp_node_pr, search_return.search_num);//合并两个节点
                    if(temp_node_pr->key_node_num<(ceil(degree/2))){
                        merge_state.is_unfull=unfull;
                        return merge_state;
                    }
                }
            }
            merge_state.is_unfull=full;
            return merge_state;
        }//内部结点再其他孩子结点中去找
    }//若为内部结点是往下查找
    else{
        search_return_state serch_return=SearchKey(data, temp_node_pr);
        if(serch_return.search_success==lose){
            printf("没有%d可以删除\n",data);
        }//查找失败没有值可以删除
        else{
            int i=serch_return.search_num;
            while (i<temp_node_pr->key_node_num-1) {
                temp_node_pr->after_first_childnode_pr[i]=temp_node_pr->after_first_childnode_pr[i+1];
                ++i;
            }
            --temp_node_pr->key_node_num;
            if(temp_node_pr->key_node_num<(ceil(degree/2))){
                merge_state.is_unfull=unfull;
                return merge_state;
            }//结点删除之后不满了
        }//查找成功删除
        merge_state.is_unfull=full;
        return merge_state;
    }//若为叶子结点时查找并删除
}

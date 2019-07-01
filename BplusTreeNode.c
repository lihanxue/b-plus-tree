//
//  xxx.c
//  b+tree
//
//  Created by 李寒雪 on 2019/5/13.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//

#include "BplusTree.h"
#include <math.h>
/*********初始化结点**********/
void InitNode(node *node){
    node->first_childnode_pr=NULL;
    node->next_leafnode_pr=NULL;
    node->key_node_num=0;
}

/*********结点中查找关键值并返回对应关键值点下标和是否找到若没有找到返回接下来查找孩子指针位置的下标*******/
search_return_state SearchKey(int data,node *temp_node_pr){
    search_return_state r;
    int i;
    for(i=0;i<temp_node_pr->key_node_num;++i){
        if(data<temp_node_pr->after_first_childnode_pr[i].key){
            r.search_success=lose;
            r.search_num=i-1;
            return r;
        }
        else if(data==temp_node_pr->after_first_childnode_pr[i].key){
            r.search_success=success;
            r.search_num=i;
            return r;
        }
    }
    r.search_success=lose;
    r.search_num=i-1;
    return r;
}

/*********B+树结点c向下分裂****/
split_state BplusTreeSplit(node *temp_node_pr){
    split_state split_state_return;
    if(temp_node_pr->state==inside_node){
        node *new_node_pr = (node*)malloc(sizeof(node));
        new_node_pr->state=inside_node;
        new_node_pr->first_childnode_pr=temp_node_pr->after_first_childnode_pr[(int)((temp_node_pr->key_node_num-1)/2)].childnode_pr;
        new_node_pr->next_leafnode_pr=NULL;
        for(int i=(int)((temp_node_pr->key_node_num-1)/2)+1,j=0 ; i<=temp_node_pr->key_node_num-1 ; ++i,++j){
            new_node_pr->after_first_childnode_pr[j]=temp_node_pr->after_first_childnode_pr[i];
        }
        new_node_pr->key_node_num=(int)(temp_node_pr->key_node_num/2);
        
        split_state_return.is_split=yes;
        split_state_return.new_node_pr=new_node_pr;
        split_state_return.up_data=temp_node_pr->after_first_childnode_pr[(int)((temp_node_pr->key_node_num-1)/2)].key;
        temp_node_pr->key_node_num=(int)((temp_node_pr->key_node_num-1)/2);
        return split_state_return;
    }//内部结点的分裂
    else{
        node *new_node_pr = (node*)malloc(sizeof(node));
        new_node_pr->state=leaf_node;
        new_node_pr->first_childnode_pr=NULL;
        new_node_pr->next_leafnode_pr=temp_node_pr->next_leafnode_pr;
        temp_node_pr->next_leafnode_pr=new_node_pr;
        
        for(int i=(int)((temp_node_pr->key_node_num)/2),j=0 ; i<=temp_node_pr->key_node_num-1 ; ++i,++j){
            new_node_pr->after_first_childnode_pr[j]=temp_node_pr->after_first_childnode_pr[i];
        }
        new_node_pr->key_node_num=(int)((temp_node_pr->key_node_num+1)/2);
        
        split_state_return.is_split=yes;
        split_state_return.new_node_pr=new_node_pr;
        split_state_return.up_data=new_node_pr->after_first_childnode_pr[0].key;
        temp_node_pr->key_node_num=(int)((temp_node_pr->key_node_num)/2);
        return split_state_return;
    }//叶子结点的分裂
}
/****B+树对不满结点时借操作***********/
borrow_state BplusTreeBorrowInsideNode(node *temp_node_pr,int unfull_node_flag){//-1为第一个孩子指针下的结点不满
    borrow_state borrow_state;
    if(unfull_node_flag-1>=-1){
        if(unfull_node_flag-1==-1){
            if(temp_node_pr->first_childnode_pr->key_node_num>(ceil(degree/2))){
                int i=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num-1;
                while(i>=0){
                    temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i+1]=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i];//1
                    --i;
                }
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i+1].childnode_pr=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->first_childnode_pr;
                    
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i+1].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag].key;//2
                    
                ++temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num;

                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->first_childnode_pr=temp_node_pr->first_childnode_pr->after_first_childnode_pr[temp_node_pr->first_childnode_pr->key_node_num-1].childnode_pr;
                
                
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].key=temp_node_pr->first_childnode_pr->after_first_childnode_pr[temp_node_pr->first_childnode_pr->key_node_num-1].key;//3
                
                --temp_node_pr->first_childnode_pr->key_node_num;
                borrow_state.can_barrow=can;
                return borrow_state;
            }//有得借
        }//左结点是第一个孩子结点
        else{
            if(temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num>((int)(ceil(degree/2)))){
                int i=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num-1;
                while(i>=0){
                    temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i+1]=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i];//1
                    --i;
                }
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i+1].childnode_pr=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->first_childnode_pr;
                
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i+1].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag].key;//2
                
                ++temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num;
                
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->first_childnode_pr=temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num-1].childnode_pr;
                
                
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].key=
                    temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num-1].key;//3
                
                --temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num;
                borrow_state.can_barrow=can;
                return borrow_state;
            }
        }//左结点不是第一个孩子节点
    }//有左结点时找左结点借
    if(unfull_node_flag+1<=temp_node_pr->key_node_num-1){
        if(unfull_node_flag==-1){
            if(temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num>((int)(ceil(degree/2)))){
                
                temp_node_pr->first_childnode_pr->after_first_childnode_pr[temp_node_pr->first_childnode_pr->key_node_num].key=temp_node_pr->after_first_childnode_pr[0].key;//1
                
                temp_node_pr->first_childnode_pr->after_first_childnode_pr[temp_node_pr->first_childnode_pr->key_node_num].childnode_pr=temp_node_pr->after_first_childnode_pr[0].childnode_pr->first_childnode_pr;
                
                ++temp_node_pr->first_childnode_pr->key_node_num;
                
                temp_node_pr->after_first_childnode_pr[0].key=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[0].key;//2
                
                temp_node_pr->after_first_childnode_pr[0].childnode_pr->first_childnode_pr=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[0].childnode_pr;
                
                int i=0;
                while (i<=temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num-2) {
                    temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[i]=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[i+1];
                    ++i;
                }
                
                --temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num;//3
                borrow_state.can_barrow=can;
                return borrow_state;
                
            }//有得借
        }//当前不满结点是第一个孩子结点
        else{
            if(temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->key_node_num>((int)(ceil(degree/2)))){
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].key;//1
                
                
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num].childnode_pr=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->first_childnode_pr;
                
                ++temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num;
                
                temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[0].key;//2
                
                temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->first_childnode_pr=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[0].childnode_pr;
                
                int i=0;
                while (i<=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->key_node_num-2){
                    temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[i]=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[i+1];//3
                    ++i;
                }
                
                --temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->key_node_num;
                borrow_state.can_barrow=can;
                return borrow_state;
            }//右结点有得借
        }//当前不满结点不是第一个孩子结点
    }//有右结点是找右结点借
    borrow_state.can_barrow=cannot;//左右结点都没有时
    return borrow_state;
}
borrow_state BplusTreeBorrowLeafNode(node *temp_node_pr,int unfull_node_flag){
    borrow_state borrow_state;
    if(unfull_node_flag-1>=-1){
        if(unfull_node_flag-1==-1){
            if(temp_node_pr->first_childnode_pr->key_node_num>(ceil(degree/2))){
                temp_node_pr->after_first_childnode_pr[0].key=temp_node_pr->first_childnode_pr->after_first_childnode_pr[temp_node_pr->first_childnode_pr->key_node_num-1].key;
                int i=temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num;
                while (i>0) {
                    temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[i].key=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[i-1].key;
                    --i;
                }
                temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[0].key=temp_node_pr->first_childnode_pr->after_first_childnode_pr[temp_node_pr->first_childnode_pr->key_node_num-1].key;
                --temp_node_pr->first_childnode_pr->key_node_num;
                ++temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num;
                borrow_state.can_barrow=can;
                return borrow_state;
            }
        }
        if(temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num>(ceil(degree/2))){
            temp_node_pr->after_first_childnode_pr[unfull_node_flag].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num-1].key;
            int i=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num;
            while (i>0) {
                temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[i-1].key;
                --i;
            }
            temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[0].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num-1].key;
            --temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num;
            ++temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num;
            borrow_state.can_barrow=can;
            return borrow_state;
        }
    }
    if(unfull_node_flag+1<=temp_node_pr->key_node_num-1){
        if(unfull_node_flag==-1){
            if(temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num>(ceil(degree/2))){
                
                temp_node_pr->first_childnode_pr->after_first_childnode_pr[temp_node_pr->first_childnode_pr->key_node_num].key=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[0].key;
                int i=0;
                while (i<temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num-1) {
                    temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[i].key=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[i+1].key;
                    ++i;
                }
                ++temp_node_pr->first_childnode_pr->key_node_num;
                --temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num;
                temp_node_pr->after_first_childnode_pr[0].key=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[0].key;
                borrow_state.can_barrow=can;
                return borrow_state;
            }
        }
        if(temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->key_node_num>(ceil(degree/2))){
            
            temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[0].key;
            int i=0;
            while (i<temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->key_node_num-1) {
                temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[i].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[i+1].key;
                ++i;
            }
            ++temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->key_node_num;
            --temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->key_node_num;
            temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].key=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->after_first_childnode_pr[0].key;
            borrow_state.can_barrow=can;
            return borrow_state;
        }
    }
    borrow_state.can_barrow=cannot;
    return borrow_state;
}

borrow_state BplusTreeBorrow(node* temp_node_pr,int unfull_node_flag){
    borrow_state borrow_state;
    if(unfull_node_flag==-1){
        if(temp_node_pr->first_childnode_pr->state==inside_node){
            borrow_state = BplusTreeBorrowInsideNode(temp_node_pr, unfull_node_flag);
        }
        else
            borrow_state=BplusTreeBorrowLeafNode(temp_node_pr, unfull_node_flag);
    }
    else{
        if(temp_node_pr->after_first_childnode_pr[unfull_node_flag].childnode_pr->state==inside_node){
            borrow_state = BplusTreeBorrowInsideNode(temp_node_pr, unfull_node_flag);
        }
        else
            borrow_state = BplusTreeBorrowLeafNode(temp_node_pr,unfull_node_flag);
    }
    return borrow_state;
}


void BplusTreeMerge(node *temp_node_pr,int unfull_node_flag){
    merge_flag merge_flag =BplusTreeMergeNodeFlag(temp_node_pr, unfull_node_flag);
    if(temp_node_pr->after_first_childnode_pr[merge_flag.next_node_flag].childnode_pr->state==inside_node){
        BpulsTreeMergeInsideNode(temp_node_pr, merge_flag.pro_node_flag, merge_flag.next_node_flag);
    }
    else{
        BplusTreeMergeLeafNode(temp_node_pr, merge_flag.pro_node_flag, merge_flag.next_node_flag);
    }
}


merge_flag BplusTreeMergeNodeFlag(node *temp_node_pr,int unfull_node_flag){
    merge_flag merge_flag;
    if(unfull_node_flag-1>=-1&&unfull_node_flag+1<=temp_node_pr->key_node_num-1){
        if(unfull_node_flag-1==-1){
            if(temp_node_pr->first_childnode_pr->key_node_num<=temp_node_pr->after_first_childnode_pr[1].childnode_pr->key_node_num){
                merge_flag.pro_node_flag=-1;
                merge_flag.next_node_flag=0;
                return merge_flag;
            }
            else{
                merge_flag.pro_node_flag=0;
                merge_flag.next_node_flag=1;
                return merge_flag;
            }
        }
        else{
            if(temp_node_pr->after_first_childnode_pr[unfull_node_flag-1].childnode_pr->key_node_num<=temp_node_pr->after_first_childnode_pr[unfull_node_flag+1].childnode_pr->key_node_num){
                merge_flag.pro_node_flag=unfull_node_flag-1;
                merge_flag.next_node_flag=unfull_node_flag;
                return merge_flag;
            }
            else{
                merge_flag.pro_node_flag=unfull_node_flag;
                merge_flag.next_node_flag=unfull_node_flag+1;
                return merge_flag;
            }
        }
    }//左右孩子都存在
    else if(unfull_node_flag-1>=-1&&unfull_node_flag+1>=temp_node_pr->key_node_num){
        merge_flag.pro_node_flag=unfull_node_flag-1;
        merge_flag.next_node_flag=unfull_node_flag;
        return merge_flag;
    }//左兄弟存在，右兄弟不存在
    else{
        merge_flag.pro_node_flag=unfull_node_flag;
        merge_flag.next_node_flag=unfull_node_flag+1;
        return merge_flag;
    }
}

void BpulsTreeMergeInsideNode(node *temp_node_pr,int pro_node_flag,int next_node_flag){
    if(pro_node_flag==-1){
        int i=temp_node_pr->first_childnode_pr->key_node_num;
        int m=temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num;
        int j=0;
        temp_node_pr->first_childnode_pr->after_first_childnode_pr[i].key=temp_node_pr->after_first_childnode_pr[0].key;
        temp_node_pr->first_childnode_pr->after_first_childnode_pr[i].childnode_pr=temp_node_pr->after_first_childnode_pr[0].childnode_pr->first_childnode_pr;
        while (j<m) {
            temp_node_pr->first_childnode_pr->after_first_childnode_pr[i+j+1]=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[j];
            ++j;
        }
        temp_node_pr->first_childnode_pr->key_node_num=temp_node_pr->first_childnode_pr->key_node_num+m+1;
        //temp_node_pr->first_childnode_pr->next_leafnode_pr=temp_node_pr->after_first_childnode_pr[0].childnode_pr->next_leafnode_pr;//1
        free(temp_node_pr->after_first_childnode_pr[0].childnode_pr);//2
        i=0;
        while (i<temp_node_pr->key_node_num-1) {
            temp_node_pr->after_first_childnode_pr[i]=temp_node_pr->after_first_childnode_pr[i+1];
            ++i;
        }
        --temp_node_pr->key_node_num;//3
    }//第一个和第二个合并
    else{
        int i=temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num;
        int m=temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr->key_node_num;
        int j=0;
        temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num].key=temp_node_pr->after_first_childnode_pr[next_node_flag].key;
        temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->after_first_childnode_pr[temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num].childnode_pr=temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr->first_childnode_pr;
        while (j<m) {
            temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->after_first_childnode_pr[i+j+1]=temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr->after_first_childnode_pr[j];
            ++j;
        }
        temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num=temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num+m+1;
        //temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->next_leafnode_pr=temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr->next_leafnode_pr;//1
        free(temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr);//2
        i=next_node_flag;
        while (i<temp_node_pr->key_node_num-1) {
            temp_node_pr->after_first_childnode_pr[i]=temp_node_pr->after_first_childnode_pr[i+1];
            ++i;
        }
        --temp_node_pr->key_node_num;//3
    }
}

void BplusTreeMergeLeafNode(node *temp_node_pr,int pro_node_flag,int next_node_flag){
    if(pro_node_flag==-1){
        int i=temp_node_pr->first_childnode_pr->key_node_num;
        int m=temp_node_pr->after_first_childnode_pr[0].childnode_pr->key_node_num;
        int j=0;
        while (j<m) {
            temp_node_pr->first_childnode_pr->after_first_childnode_pr[i+j]=temp_node_pr->after_first_childnode_pr[0].childnode_pr->after_first_childnode_pr[j];
            ++j;
        }
        temp_node_pr->first_childnode_pr->key_node_num=temp_node_pr->first_childnode_pr->key_node_num+m;
        temp_node_pr->first_childnode_pr->next_leafnode_pr=temp_node_pr->after_first_childnode_pr[0].childnode_pr->next_leafnode_pr;//1
        free(temp_node_pr->after_first_childnode_pr[0].childnode_pr);//2
        i=0;
        while (i<temp_node_pr->key_node_num-1) {
            temp_node_pr->after_first_childnode_pr[i]=temp_node_pr->after_first_childnode_pr[i+1];
            ++i;
        }
        --temp_node_pr->key_node_num;//3
    }//叶子孩子结点为第一个结点
    else{
        int i=temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num;
        int m=temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr->key_node_num;
        int j=0;
        while (j<m) {
            temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->after_first_childnode_pr[i+j]=temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr->after_first_childnode_pr[j];
            ++j;
        }
        temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num=temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->key_node_num+m;
        temp_node_pr->after_first_childnode_pr[pro_node_flag].childnode_pr->next_leafnode_pr=temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr->next_leafnode_pr;//1
        free(temp_node_pr->after_first_childnode_pr[next_node_flag].childnode_pr);//2
        i=next_node_flag;
        while (i<temp_node_pr->key_node_num-1) {
            temp_node_pr->after_first_childnode_pr[i]=temp_node_pr->after_first_childnode_pr[i+1];
            ++i;
        }
        --temp_node_pr->key_node_num;//3
    }//合并的两个结点中没有第一个结点
}


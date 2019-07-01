//
//  xxx.h
//  b+tree
//
//  Created by 李寒雪 on 2019/5/13.
//  Copyright © 2019年 lhx.uestc.cd. All rights reserved.
//
#ifndef _BPLUSTREE_BPLUSTREE_H_
#define _BPLUSTREE_BPLUSTREE_H_

#include <stdio.h>
#include <stdlib.h>
#define degree 50

enum search_state{success=1,lose=0};
enum is_split{yes=1,no=0};
enum is_unfull{unfull=1,full=0};
enum can_borrow{can=1,cannot=0};

typedef struct search_return_state{
    enum search_state search_success;
    int search_num;
}search_return_state;


enum node_state{leaf_node=0,inside_node=1};
typedef struct key_node{
    int key;//关键值
    struct node *childnode_pr;//关键值结点的孩子结点指针
}key_node;//关键值结点结构体

key_node after_first_childpr[degree];//关键值结点数组

typedef struct node{
    struct node *first_childnode_pr;//第一个孩子结点指针
    key_node after_first_childnode_pr[degree];//第一个孩子结点之后的部分是一个关键值结点的数组
    int key_node_num;
    enum node_state state;
    struct node *next_leafnode_pr;//下一个叶子结点指针
}node;                  //结点

typedef struct split_state{
    enum is_split is_split;
    node *new_node_pr;
    int up_data;
}split_state ;

typedef struct merge_state{
    enum is_unfull is_unfull;
}merge_state;

typedef struct borrow_state{
    enum can_borrow can_barrow;
}borrow_state;

typedef struct merge_flag{
    int pro_node_flag;
    int next_node_flag;
}merge_flag;

typedef struct leve_order_node{
    int deepth_flag;
    node *node_pr;
    struct leve_order_node *next_node_pr;
}leve_order_node;

/*********初始化结点**********/
void InitNode(node *node);

/*********结点中查找关键值并返回对应关键值点下标和是否找到若没有找到返回带插入下标位置*******/
search_return_state SearchKey(int data,node *temp_node_pr);

/*********B+树结点c向下分裂****/
split_state BplusTreeSplit(node *temp_node_pr);

/**********B+树关键值插入******/
split_state BplusTreeInsertData(int data,node *temp_node_pr);

/*********B+树的创建*********/
node* CreatBplusTree(int data[1],int data_num);

/*********B树结点借关键值********/
borrow_state BplusTreeBorrow(node *temp_node_pr,int unfull_node_flag);

/*********找B+树下溢结点与哪个结点合并*******/
merge_flag BplusTreeMergeNodeFlag(node *temp_node_pr,int unfull_node_flag);

/*********B+树内部结点合并******/
void BpulsTreeMergeInsideNode(node *temp_node_pr,int pro_node_flag,int next_node_flag);

/********B+树叶子节点合并******/
void BplusTreeMergeLeafNode(node *temp_node_pr,int pro_node_flag,int next_node_flag);

/*********B+树结点的合并*****/
void BplusTreeMerge(node *temp_node_pr,int unfull_node_flag);

/******B+树结点的关键值删除******/
merge_state BplusTreeDelete(int data,node *temp_node_pr);

/******摧毁一颗B+树**********/
node* BplusTreeTrash(node *root_node_pr,int data[],int data_num);


/******输出B+树结点的关键字********/
void PrintBplusTreeNode(node *temp_pr,FILE *fp);

/*********B+树的层次遍历**********/
void BplusTreeLeveOrder(node *root_node_pr);

/********B+树的关键字查找**********/
search_return_state BplusTreeSearch(int data,node *root_node_pr);



#endif // _BPLUSTREE_BPLUSTREE_H_

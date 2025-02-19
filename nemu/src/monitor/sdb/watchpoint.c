/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "sdb.h"

#define NR_WP 32
typedef struct watchpoint {
  int NO;
  char expr[128];
  int old_value;
  int new_value;   // 新增：记录最新的值
  struct watchpoint *next;
} WP;
static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;


void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}
//new a watchpoint
WP* new_wp(void)
{

  //创建第一个监视点的情况
  if(head==NULL)
  {
    head=&wp_pool[0];
    free_=&wp_pool[1];
    return head;
  }
  else//创建多个监视点时(已经存在head)
  {
    WP *temp=free_;
    free_=free_->next;
    return temp;
  }
}

void free_wp(WP *wp)
{

  if(wp!=head)//删除的节点不是头节点
  { //因为删除后会挂载到最后，无需判断是否只有头结点
    //获取上一个节点wp_pool[num]
    int num=-1;
    for(int i=0;i<NR_WP;i++)
    {
      if(wp_pool[i].next==wp)
      {
          num=i;
          break;
      }
    }
    assert(num!=-1);//判断z是否找到了上一个变量
    //修改上一个节点next部分
    wp_pool[num].next=wp->next;
    //清空free结构体的内容
    memset(wp->expr,0,sizeof(wp->expr));
    wp->next=NULL;
    wp->new_value=0;
    wp->old_value=0;
    //将该内容挂载到最后
    for(int i=0;i<NR_WP;i++)
    {
      if(wp_pool[i].next==NULL&&wp_pool[i].NO!=wp->NO)
      {
        wp_pool[i].next=wp;
        break;
      }
    }
  }
  else//删除头结点
  {
    //存储头结点的信息
    WP *temp=head;
    //头结点更新
    head=head->next;
    if(head==free_)//判断是否是只有头结点
    {
      return;
    }
    //清空原本head结构体的内容
    memset(temp->expr,0,sizeof(temp->expr));
    temp->next=NULL;
    temp->new_value=0;
    temp->old_value=0;
    //将原本的头结点挂载到最后
    for(int i=0;i<NR_WP;i++)
    {//找到原本的最后一个链表
      if(wp_pool[i].next==NULL&&wp_pool[i].NO!=temp->NO)
      {//挂载
        wp_pool[i].next=temp;
        break;
      }      
    }    
  }
}

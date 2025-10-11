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
WP* new_wp(void);
void free_wp(WP *wp);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}
void try(void)
{}
//删除某个监视点
void d_watchpoint(int num)
{
  assert(num>=0&&num<=32);
  //检测是否为使用中的监视点
  WP* temp=head;
  WP* using_watchpoint=NULL;
  int using_flag=0;//0表示i没有使用
  for(temp=head;temp!=free_;temp=temp->next)
  {
    if(num==temp->NO)
    {
      using_flag=1;
      using_watchpoint=temp;
      break;
    }
  }
  if(using_flag==1)
  {
    printf("free watchpoint %d is successful\n",using_watchpoint->NO);
    free_wp(using_watchpoint);
  }else
  {
    printf("This watchpoint is not being used\n");
  } 
}
int add_watchpoint(char *expr)
{
    WP* temp=new_wp();//新建一个监视点
    int num=temp->NO;//得到所分配链表的编号
    strcpy(temp->expr, expr);//拷贝表达式
    printf("watchpoint add expr is %s\n",temp->expr);
    return num;//返回编号
}
void watchpoint_show(void)
{
  WP* temp=head;
  for(temp=head;temp!=free_;temp=temp->next)
  {
    printf("breakpoint %d is %s,old is %d,new is %d\n",temp->NO,temp->expr,temp->old_value,temp->new_value);
  }
}
//更新监视点的值，在cpu-exec中只需要调用即可,如果有改变的话返回1，没有变化的话返回0
int watchpoint_updata(void)
{
  WP* temp=head;
  word_t result=0;//表达式求值的结果
  int change_flag=0;//表明是否发生改变
  bool *suc=malloc(sizeof(bool));
  //判断是否存在watchpoint
  if(head!=NULL)
  {
    while(1)
    {
      result=expr(temp->expr,suc);
      temp->old_value=temp->new_value;
      temp->new_value=result;
      if(temp->old_value!=temp->new_value)
      {
        change_flag=1;
      }
      temp=temp->next;
      if(temp==free_)
      {
        break;
      }
    }
    if(change_flag==1)
    {
      WP* PUT;
      for(PUT=head;(PUT!=free_)&&(PUT->next!=NULL);PUT=PUT->next)
      {
        printf("for %d point,the expr is %s\nthe old data is %x,the new data is %x\n", PUT->NO,PUT->expr,PUT->old_value,PUT->new_value);
      }
    }
    return change_flag;
  }
  else//没有监视点的话就返回-1
  {
    return -1;
  }
  
}
//新建一个监视点
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
